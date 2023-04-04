# Parallel RSA file encoding
File encoding programm (Parallel RSA). Designed with Qt Creator

Desktop app designed with Qt Creator to encode your files.

- [x] Converting files to bytes vector
- [x] Restoring files from bytes vector
- [x] RSA with paralleling execution (Open MP)
- [ ] Qt-based UI


## The basic concept
#### encoding:
1. Convert selected file to bytes vector
2. Encode bytes using parallel RSA (with saving file extension)
3. Return encoded bytes file
 
#### decoding:
1. Convert selected encoded bytes file to bytes vector
2. Decode encoded bytes to original bytes vector (Parallel RSA)
3. **[PROFIT]**. Return decoded file with the same file extension 
