use crate::messages::Message;
use crate::wscript_jsrt::WScript;
use std::cell::Cell;
use std::cmp::Ordering;
use std::collections::BinaryHeap;
use std::sync::Mutex;
use std::time::{Duration, Instant};

#[derive(Eq, PartialEq)]
struct Entry {
    message: Message,
    scheduled_time: Instant,
    id: u32,
    cancelled: Cell<bool>,
}

impl PartialOrd for Entry {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Entry {
    fn cmp(&self, other: &Self) -> Ordering {
        other.scheduled_time.cmp(&self.scheduled_time)
    }
}

pub struct MessageQueue {
    messages: Mutex<BinaryHeap<Entry>>,
}

impl MessageQueue {
    pub(crate) fn new() -> Self {
        Self {
            messages: Mutex::default(),
        }
    }

    #[tracing::instrument(skip_all)]
    pub(crate) fn insert_sorted(&self, msg: Message) -> u32 {
        // TODO: remove get_id from message. id should only be here.
        let id = msg.get_id();
        let delay = Duration::from_millis(msg.get_delay_ms() as u64);
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

    // TODO: don't need remove_all since the queue will clear on drop
    #[tracing::instrument(skip_all)]
    pub fn remove_all(&self) {
        let mut lease = self.messages.lock().unwrap();
        lease.clear();
    }
}
