#include <stdint.h>


#ifdef __linux__
#include <gd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <sys/stat.h>

#define debug(S) printf(S)

#else
#define debug(S, ...)
#include "fb.h"
#endif

#define LCD_XSIZE 132

typedef struct{
    uint8_t width;
    uint8_t posX;
    uint8_t posY;
} image_frame;

enum compression_standard {
    RLE = 0x00,
    COLOR_PALLET = 0x01,
    RAW = 0x02,
};

typedef uint16_t sizeField;

typedef struct {
    sizeField totalBytes;
    uint8_t standard;
    uint8_t numberOfColors;
    uint8_t numberOfImageFrames;
} compression_image_header;

typedef struct {
    uint8_t width; //minimum bounding box of it's subframes
    uint8_t height;
} image;

typedef struct __attribute__((packed)) {
    unsigned r:5;
    unsigned g:6; //We actually have 6 bits for green instead of 5
    unsigned b:5;
} pixel;

#define COLOR_LIST_SIZE 251
#define CODE_EXTENDED 252
#define CODE_REPEAT 253
#define CODE_TRANSPARENT 254
#define CODE_RGB 255

pixel colorList[COLOR_LIST_SIZE] = {
        {0,0,0},
        {31,63,31},
        {31,4,31}};

void writeToBuffer(int x, int y, pixel s) {
    if(x < 0 || x >= LCD_XSIZE ||
            y < 0 || y >= LCD_XSIZE) { //Change for non square screens
        return;
    }
    
#ifndef __linux__
    G_Fb.buffer[y*LCD_XSIZE+x] = *(uint16_t*)(&s);
#else
    printf("Wrote a pixel! r%d g%d b%d\n",s.r,s.g,s.b);
#endif
    
    
}


void renderCompressedImage(unsigned char data[], unsigned char x, unsigned char y, unsigned char sub_frame) {
    
    debug("Attempting to decode compressed image!\n");
    compression_image_header header;
    int  bP = 0;
    header.totalBytes = *(sizeField*)(&data[bP]);
    bP += sizeof(sizeField);
    header.standard = *(uint8_t*)(&data[bP]);
    bP += sizeof(uint8_t);
    header.numberOfColors = *(uint8_t*)(&data[bP]);
    bP += sizeof(uint8_t);
    header.numberOfImageFrames = *(uint8_t*)(&data[bP]);
    bP += sizeof(uint8_t);
    
    pixel localColors[header.numberOfColors];
    
    int i;
    for(i = 0; i < header.numberOfColors; ++i) {
        debug("Found a custom color!\n");
        localColors[i].r = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint8_t);
        localColors[i].g = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint8_t);
        localColors[i].b = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint8_t);
    }
    
    for(i = 0; i < header.numberOfImageFrames; ++i) {
        image_frame current_frame;
        uint16_t image_frame_size = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint16_t);
        //image_frame_size += 3; Pretty sure I do not need this
        
        current_frame.width = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint8_t);
        
        current_frame.posX = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint8_t);
        
        current_frame.posY = *(uint8_t*)(&data[bP]);
        bP += sizeof(uint8_t);
        
        int frame_data_itr;
        pixel current_pixel_color;
        int repeat_offset = 0;
        unsigned char transparency_flag = 0;
        
        debug("Found a sub frame!\n");
        
        for(frame_data_itr; frame_data_itr < image_frame_size-5; //The 5 comes from the size of the frame 
                ++frame_data_itr)
        {
            unsigned char cbyte = data[bP+frame_data_itr];
            
            int frame_itr_x = (frame_data_itr+repeat_offset)%current_frame.width;
            int frame_itr_y = (frame_data_itr+repeat_offset)/current_frame.width;
            
            if(cbyte < COLOR_LIST_SIZE) {
                current_pixel_color = colorList[cbyte];
                transparency_flag = 0;
            } else {
                switch(cbyte) {
                case CODE_EXTENDED: {
                    if(data[bP+frame_data_itr+1] < header.numberOfColors) {
                        current_pixel_color = localColors[bP+frame_data_itr+1];
                    }
                    frame_data_itr += 1; //Skip the next byte
                    repeat_offset -= 1; //Set the proper offset

                }
                break;
                case CODE_REPEAT: {
                    unsigned char i;
                    debug("Found repeat code!\n");
                    if(transparency_flag) {
                        repeat_offset += data[bP+frame_data_itr+1];
                        continue;
                    }
                    for(i = 0; i < data[bP+frame_data_itr+1]; ++i) {
                        writeToBuffer(current_frame.posX + frame_itr_x + x,
                            current_frame.posY + frame_itr_y + y, current_pixel_color);
                        repeat_offset++;
                        frame_itr_x = (frame_data_itr+repeat_offset)%current_frame.width;
                        frame_itr_y = (frame_data_itr+repeat_offset)/current_frame.width;
                    }
                    frame_data_itr += 1; //Skip the next byte
                    repeat_offset -= 1; //Set the proper offset
                }
                break;
                case CODE_TRANSPARENT: {
                    transparency_flag = 1;
                    continue;
                }
                break;
                case CODE_RGB: {
                    debug("Found a custom pixel color!\n");
                    transparency_flag = 0;
                    current_pixel_color.r = *(uint8_t*)(&data[bP+frame_data_itr+1]);
                    current_pixel_color.g = *(uint8_t*)(&data[bP+frame_data_itr+2]);
                    current_pixel_color.b = *(uint8_t*)(&data[bP+frame_data_itr+3]);
                    frame_data_itr += 3; //Skip the next byte
                    repeat_offset -= 3; //Set the proper offset
                }
                break;
                default:
                    current_pixel_color = colorList[cbyte];
                    transparency_flag = 0;
                }
            }
            writeToBuffer(current_frame.posX + frame_itr_x + x,
                        current_frame.posY + frame_itr_y + y, current_pixel_color);
        }
        bP += image_frame_size;
    }
}

typedef struct {
    int idx;
    int error;
} matchResults;

matchResults matchColor(pixel table[], int tableSize, pixel color) {
    matchResults r;
    r.error = 255+255+255;
    r.idx = 0;
    int i;
    
    //This is really basic and you can really make this more efficient 
    for(i = 0; i < tableSize; ++i) {
        int current_error = abs(table[i].r-color.r)
                          + abs(table[i].g-color.g)
                          + abs(table[i].b-color.b);
        if(current_error < r.error) { //We found a better result!
            r.error = current_error;
            r.idx = i;
        }
    }
    return r;
}

int encode_simple_image(pixel pixels[], int count, int width, 
        unsigned char dest[], int sizeDest) {
    compression_image_header header;
    header.numberOfColors = 0;
    header.numberOfImageFrames = 1;
    header.standard = RLE;
    header.totalBytes = 0;
    
    image_frame frame;
    frame.posX = 0;
    frame.posY = 0;
    frame.width = width;
        
    unsigned char buffer[400]; //TODO set the buffer size to variable
    int bP = 0;
    
    int i;
    for(i = 0; i < count; ++i) {
        
        buffer[bP] = matchColor(colorList, COLOR_LIST_SIZE, pixels[i]).idx;
        
        //Perform lookahead
        int peek_idx;
        for(peek_idx = 1; peek_idx+i < count || peek_idx < sizeof(unsigned char)-2; ++peek_idx) {
            int matched_color = matchColor(colorList, COLOR_LIST_SIZE, pixels[i+peek_idx]).idx;
            if(buffer[bP] != matched_color) {
                break;
            }
        }
        
        if(peek_idx > 3){
            buffer[bP+1] = CODE_REPEAT;
            buffer[bP+2] = (unsigned char)(peek_idx-2);
            bP+=2;
            i += peek_idx;
        }
        
        bP++;
    }
    
    
    //Calculate memory sizes
    uint16_t frameSize = bP + sizeof(uint16_t) + sizeof(frame.posX) + sizeof(frame.posY) 
        + sizeof(frame.width);
    
    int imageSize = sizeof(header.numberOfColors);
    imageSize += sizeof(header.numberOfImageFrames);
    imageSize += sizeof(header.standard);
    imageSize += sizeof(header.totalBytes);
    imageSize += frameSize;
    
    header.totalBytes = imageSize;
    
    
    if(header.totalBytes > sizeDest) {
        //Panic we do not have enough space in our destination buffer
        debug("Not enough memory to save our image!!");
        return -1;
    }
    int dP = 0;
    
#define bufcpy(D,P,V) memcpy(&D[P],&V,sizeof(V)); P += sizeof(V)
    
    bufcpy(dest,dP,header.totalBytes);
    bufcpy(dest,dP,header.standard);
    bufcpy(dest,dP,header.numberOfColors);
    bufcpy(dest,dP,header.numberOfImageFrames);
    //TODO: Add local color table when needed
    bufcpy(dest,dP,frameSize);
    bufcpy(dest,dP,frame.width);
    bufcpy(dest,dP,frame.posX);
    bufcpy(dest,dP,frame.posY);
    
    //
    memcpy(&dest[dP],buffer,bP);
    dP += bP;
    //Fin!
    
    //(sizeField)&dest[dP] = header.totalBytes;
    
    //save to destination memory region
    return dP;
}

/*
void renderCompressedImage(unsigned char data[], unsigned char x, unsigned char y) {
    renderCompressedImage(data,x,y,0);
}*/



#ifdef __linux__




#define test_list_size 20
int performTests() {
    
    printf("Testing Compression Library!\n");
    
    pixel plist[test_list_size] = {
        {15,2,15},
        {15,63,15},
        {15,63,15},
        {15,63,15},
        {15,63,15},
    };
    
    unsigned char b[200] = {10};
    int size = encode_simple_image(plist,5,5,b,200);
    
    printf("Compressed Image Data of size: %d\n",size);
    printf("//C array for compressed image\n");
    printf("const unsigned char[] = { ");
    int i;
    for(int i = 0; i < size; i++) {
        
        printf("0x%x,",b[i]);
        if(i%5 == 0) {
            printf("\n");
        }
    }
    printf("};\n");
    
    unsigned char test[] = { 0xe,
0x0,0x0,0x0,0x1,0x9,
0x0,0x5,0x0,0x0,0x0,
0x1,0xfd,0x2,};
    
    renderCompressedImage(test,0,0,0);
}


int main(int argc, char *argv[]) {
    if(argc != 2) {
        performTests();
        return 0;
    }
    FILE *in = fopen(argv[1], "rb");
    gdImagePtr im;
    struct stat stat_buf;
    if(!in) {
        return -1;
    }
    if (fstat(fileno(in), &stat_buf) != 0) {
        return -1;
    }
    char *buffer = malloc(stat_buf.st_size);
    if (!buffer) {
        return -1;
    }
    if (fread(buffer, 1, stat_buf.st_size, in)
    != stat_buf.st_size)
    {
        return -1;
    }
    im = gdImageCreateFromPngPtr(stat_buf.st_size, buffer);
    free(buffer);
    fclose(in);
    
    im->sx;
    gdImageGetPixel(im,0,0);
    //Not quite finished
    
}
#endif