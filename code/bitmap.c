#pragma pack(push, 1)
typedef struct {
    WORD  file_type;
    DWORD file_size;
    WORD  reserved1;
    WORD  reserved2;
    DWORD offset;
    DWORD info_header_size;
    DWORD width;
    DWORD height;
    WORD  planes;
    WORD  bits_per_pixel;
} bitmap_header_t;
#pragma pack(pop)

uint32_t *load_bitmap(wchar_t *path_to_bitmap, int *width, int *height) {
    uint32_t *pixels = NULL;

    unsigned char *data = read_entire_file(path_to_bitmap, NULL);
    if(data) {
        bitmap_header_t *header = (bitmap_header_t *)data;
        
        if(width) *width = header->width;
        if(height) *height = header->height;
        pixels = malloc(header->file_size - header->offset);
        
        if(pixels) {
            uint32_t *color = (uint32_t *)(data + header->offset);
            for(uint32_t i = 0; i < header->width * header->height; ++i) {
                unsigned char blue  = 0xFF & (color[i]);
                unsigned char green = 0xFF & (color[i] >> 8);
                unsigned char red   = 0xFF & (color[i] >> 16);
                unsigned char alpha = 0xFF & (color[i] >> 24);
                
                pixels[i] = alpha << 24 | blue << 16 | green << 8 | red;
            }
        }
        
        free(data);
    }
    
    return(pixels);
}
