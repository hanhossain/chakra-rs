use crate::jsrt::buffer::JsArrayBuffer;
use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsParseScriptAttributes, JsSourceContext, JsValueRef};

pub trait JsRun: AsRef<JsValueRef> {
    fn serialize(&self, attributes: JsParseScriptAttributes) -> Result<JsArrayBuffer, JsError> {
        let mut buffer = JsValueRef::default();
        unsafe {
            bridge::JsSerialize(*self.as_ref(), &raw mut buffer, attributes).as_result()?;
        }
        Ok(JsArrayBuffer::new(buffer))
    }

    fn serialize_parser_state(
        &self,
        attributes: JsParseScriptAttributes,
    ) -> Result<JsArrayBuffer, JsError> {
        let mut buffer = JsValueRef::default();
        unsafe {
            bridge::JsSerializeParserState(*self.as_ref(), &raw mut buffer, attributes)
                .as_result()?;
        }
        Ok(JsArrayBuffer::new(buffer))
    }

    fn run<T: AsRef<JsValueRef>>(
        &self,
        source_context: JsSourceContext,
        source_url: T,
        parse_attributes: JsParseScriptAttributes,
    ) -> Result<(), JsError> {
        // If we pass a JsValueRef here to capture the result, Jsrt::RunScriptCore loads the
        // script as an expression and the final bytecode changes to include a return value
        // (i.e. return undefined).
        unsafe {
            bridge::JsRun(
                *self.as_ref(),
                source_context,
                *source_url.as_ref(),
                parse_attributes,
                std::ptr::null_mut(),
            )
            .as_result()?;
        }
        Ok(())
    }

    fn run_script_with_parser_state(
        &self,
        source_context: JsSourceContext,
        source_url: JsValueRef,
        parse_attributes: JsParseScriptAttributes,
        parser_state: &JsArrayBuffer,
    ) -> Result<JsValueRef, JsError> {
        let mut result = JsValueRef::default();
        unsafe {
            bridge::JsRunScriptWithParserState(
                *self.as_ref(),
                source_context,
                source_url,
                parse_attributes,
                *parser_state.as_ref(),
                &raw mut result,
            )
            .as_result()?;
        }
        Ok(result)
    }
}
