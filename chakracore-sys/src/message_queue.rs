use crate::messages::Message;
use crate::wscript_jsrt::WScript;
use std::cmp::Ordering;
use std::collections::BinaryHeap;
use std::sync::Mutex;
use std::time::Instant;

#[derive(Eq, PartialEq)]
struct Entry {
    message: Message,
    scheduled_time: Instant,
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
    queue: Mutex<BinaryHeap<Entry>>,
}

impl MessageQueue {
    pub(crate) fn new() -> Self {
        todo!()
    }

    pub(crate) fn insert_sorted(&self, msg: Box<Message>) {
        todo!()
    }

    pub(crate) fn remove_by_id(&self, timer_id: u32) {
        todo!()
    }

    pub fn is_empty(&self) -> bool {
        todo!()
    }

    pub fn process_all(&self, filename: &str, wscript: &WScript) {
        todo!()
    }

    pub fn remove_all(&self) {
        todo!()
    }
}
