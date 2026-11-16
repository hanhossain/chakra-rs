use crate::messages::Message;
use crate::wscript_jsrt::WScript;
use std::cell::Cell;
use std::cmp;
use std::collections::BinaryHeap;
use std::sync::Mutex;
use std::sync::atomic::{AtomicU32, Ordering};
use std::time::{Duration, Instant};

#[derive(Eq, PartialEq)]
struct Entry {
    message: Message,
    scheduled_time: Instant,
    id: u32,
    cancelled: Cell<bool>,
}

impl PartialOrd for Entry {
    fn partial_cmp(&self, other: &Self) -> Option<cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Entry {
    fn cmp(&self, other: &Self) -> cmp::Ordering {
        other.scheduled_time.cmp(&self.scheduled_time)
    }
}

pub struct MessageQueue {
    messages: Mutex<BinaryHeap<Entry>>,
    id_counter: AtomicU32,
}

impl MessageQueue {
    pub(crate) fn new() -> Self {
        Self {
            messages: Mutex::default(),
            id_counter: AtomicU32::new(0),
        }
    }

    #[tracing::instrument(skip(self, msg))]
    pub(crate) fn schedule_task(&self, msg: Message, delay: Duration) -> u32 {
        let id = self.id_counter.fetch_add(1, Ordering::Relaxed);
        let scheduled = Instant::now() + delay;
        let entry = Entry {
            message: msg,
            scheduled_time: scheduled,
            id,
            cancelled: Cell::new(false),
        };
        let mut lease = self.messages.lock().unwrap();
        lease.push(entry);
        // TODO: trigger refresh for waiting thread if min changed
        id
    }

    #[tracing::instrument(skip(self))]
    pub(crate) fn remove_by_id(&self, timer_id: u32) {
        let lease = self.messages.lock().unwrap();
        if let Some(entry) = lease.iter().find(|x| x.id == timer_id) {
            entry.cancelled.set(true);
            // TODO: trigger refresh for waiting thread if min changed
        }
    }

    pub fn is_empty(&self) -> bool {
        let lease = self.messages.lock().unwrap();
        lease.is_empty()
    }

    #[tracing::instrument(skip_all)]
    pub fn process_all(&self, filename: &str, wscript: &WScript) {
        while !self.is_empty() {
            let next_scheduled_time = {
                let mut lease = self.messages.lock().unwrap();
                let entry = lease.peek().unwrap();
                if entry.cancelled.get() {
                    lease.pop();
                    continue;
                }
                entry.scheduled_time
            };

            let delay = next_scheduled_time.duration_since(Instant::now());
            tracing::debug!(?delay, "Sleeping until next iteration");
            // TODO: listen to notification to update refresh if next scheduled time changed
            std::thread::sleep(delay);
            tracing::debug!("Resuming execution");

            let entry = {
                let mut lease = self.messages.lock().unwrap();
                lease.pop().unwrap()
            };
            entry.message.call(filename, wscript);
        }
    }
}
