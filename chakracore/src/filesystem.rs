#[tracing::instrument(err)]
pub(crate) fn file_to_string(path: &str) -> std::io::Result<String> {
    let bytes = std::fs::read(path)?;
    Ok(String::from_utf8_lossy(&bytes).into())
}
