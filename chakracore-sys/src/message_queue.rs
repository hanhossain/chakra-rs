pub use ffi::MessageQueue;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("MessageQueue.h");

        type MessageQueue;

        #[Self = "MessageQueue"]
        fn New() -> SharedPtr<MessageQueue>;

        fn RemoveAll(self: Pin<&mut MessageQueue>);
        fn IsEmpty(self: Pin<&mut MessageQueue>) -> bool;
        fn ProcessAll(self: Pin<&mut MessageQueue>, filename: &str) -> i32;
        fn InsertSorted(self: Pin<&mut MessageQueue>, message: Box<Message>);
        fn RemoveById(self: Pin<&mut MessageQueue>, id: u32);

        #[namespace = "chakra_rs"]
        type Message<'a> = crate::messages::Message<'a>;
    }
}
