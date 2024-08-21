unsigned char *read_entire_file(wchar_t *filename, int *size) {
    int file_size = 0;
    unsigned char *data = NULL;

    HANDLE file_handle = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(file_handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER large_int;
        if(GetFileSizeEx(file_handle, &large_int)) {
            if(large_int.QuadPart > INT_MAX) {
                CloseHandle(file_handle);
                assert(!"The file size is too big for this implementation of read_entire_file()!");
                return(data);
            }
            
            file_size = (int)large_int.QuadPart;
            data = malloc(file_size);

            if(data) {
                DWORD bytes_read;
                if(!ReadFile(file_handle, data, file_size, &bytes_read, NULL)) {
                    free(data);
                    data = NULL;
                    file_size = 0;

                    CloseHandle(file_handle);
                    assert(!"ReadFile() failed.");
                    return(data);
                }

                if(bytes_read != (DWORD)file_size) {
                    free(data);
                    data = NULL;
                    file_size = 0;

                    CloseHandle(file_handle);
                    assert(!"Bytes read is not equal to the queried file size.");
                    return(data);
                }
            }
            else {
                CloseHandle(file_handle);
                assert(!"Failed to allocate the necessary memory!");
                return(data);
            }
        }
        
        CloseHandle(file_handle);
    }
    else {
        assert(!"Failed to open the file.");
        return(data);
    }

    if(size) *size = file_size;

    return(data);
}
