#define debug_print(fmt, ...) \
  do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)


void ApTerm( void);

void winfo( WINDOW *win, int index);
void wclose( WINDOW *win, int index);
void wCheck( WINDOW *win, int index, int mode, char* appName);
void wFsel( WINDOW *win, int index);

void makeConfig(char* appName);

void AddTextLabel( void *dial, int parent, char *obj_label);
void AddCheckBox( void *dial, int parent, ezxml_t object, char *obj_label);
void AddRadioButtons( void   *dial, int     parent, ezxml_t object, char    *obj_label);
void AddButton(  void *dial, int parent, char *obj_label);

void RightButton(void);
