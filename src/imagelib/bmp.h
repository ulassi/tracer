struct bmp24_file_header
{
  short int   type;         // 'MB'
  long        size;         // 0
  short int   reserved1;    // 0
  short int   reserved2;    // 0
  long        offbits;      // 14 + 40
};

struct bmp24_info_header
{
  long	      size;                // 40
  long	      width;               // pic.width
  long	      height;              // pic.height
  short int   planes;              // 1
  short int   bit_count;           // 24
  long	      compression;         // 0
  long        size_image;          // (pic.width * 3 + extra_bytes) * pic.height
  long	      x_pels_per_meter;    // 2952
  long	      y_pels_per_meter;    // 2952
  long	      clr_used;            // 0
  long	      clr_important;       // 0
};
