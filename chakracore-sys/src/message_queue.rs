use crate::messages::Message;
use crate::wscript_jsrt::WScript;

pub struct MessageQueue;

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
