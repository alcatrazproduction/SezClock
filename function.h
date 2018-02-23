

void  writeDigit( int witch, unsigned char data );
void  writeDots( int witch, unsigned long col );
void  displayClock( void );
void  writeLed( int witch, unsigned long col );
int   processTouch( void);
int   readEEProm();
int   writeEEProm();

// Menu type
#define M_NORMAL      0x0001              // Standard menu
#define M_SPECIAL     0x0002              // Special menu
#define M_RESERVED    0x0004              // Reserved menu for devlopper
// Standard menu option
#define M_SETMODE     0x0100              // Set mode stage ( Race or clock )
#define M_SETRACE     0x0200              // Setting race ( choice from preset or modifiy )
#define M_SELPRESET   0x0400              // Select presset
#define M_SELPARAM    0x0800              // Select param to edit
#define M_EDITTIME    0x1000              // Edit time
#define M_EDITLAPS    0x2000              // Edit laps
// Special Menu
#define MS_WINTER     0x0100              // Switch time periode
#define MS_EDITCLOCK  0x0200              // Edit clock
#define MS_EDITTMZ    0x0400              // Timezone edit
#define MS_EDITHRS    0x0800              // Hours edit
#define MS_EDITMIN    0x1000              // Minutes edit
#define MS_EDITSEC    0x2000              // Seconds edit

typedef struct EEProm
{
  unsigned long   saveTime;               // Saved time
  char            timeZone;
  bool            summerTime;
  bool            reserved1;
  bool            reserved2;
  bool            reserved3;
  bool            reserved4;
  bool            reserved5;
  bool            reserved6;
  bool            reserved7;
  unsigned long   lastRace;
  unsigned long   lastLap;

} S_EEProm;

static  char  *message[]    =
{
  "Menu                ",
  "Return              ",
  "Select Mode         ",
  "Display Clock       ",
  "Race Mode           "
  "Select Preset       ",
  "Minutes             ",
  "Laps                ",
  "Start Race          ",
  "Stop Race           ",
  "Time Ended          ",
  "Press Enter for Lap ",
  "Race Ended          ",
};

static  char  *menu_0001[]   =
{
  "Menu, Select SubMenu",
  "Mode Selection      ",
  "Setting             ",
  "Race Setting        ",
};

static  char  *menu_0101[]   =
{
  "Select Mode         ",
  "Display Clock       ",
  "Race Mode           "
};

/********************************************************************************************************
 * Race definition (pre-storade model)
 * 
 */

 typedef struct
 {
  char            *name;
  bool            race;
  unsigned  long  time;
  unsigned  int   laps;
 } raceDef;

static raceDef    _racedef[]  =
{
  {
    "Essai Angora",
    false,
    20*60,
    0
  },
  {
    "Chrono MX3",
    true,
    15*60,
    0
  },
  {
    "Race 10/2",
    true,
    10*60,
    2
  },
  {
    "Race 15/2",
    true,
    15*60,
    2
  },
  {
    "Race 20/2",
    true,
    20*60,
    2
  },
  {
    "Race 25/2",
    true,
    25*60,
    2
  },
   {
    "Race 30/2",
    true,
    30*60,
    2
  },
   {
    "Race 35/2",
    true,
    35*60,
    2
  },
   {
    "Trainning 10",
    false,
    10*60,
    0
  },
   {
    "Trainning 15",
    false,
    15*60,
    0
  },
   {
    "Trainning 20",
    false,
    20*60,
    0
  },
   {
    "Trainning 25",
    false,
    25*60,
    0
  },
   {
    "Trainning 30",
    false,
    30*60,
    0
  },
   {
    "Trainning 35",
    false,
    35*60,
    0
  },
   {
    "Trainning 40",
    false,
    40*60,
    0
  },
   {
    "Trainning 45",
    false,
    45*60,
    0
  },
   {
    "Trainning 50",
    false,
    50*60,
    0
  },

};


