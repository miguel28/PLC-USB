#include "main.h"
#define ZOOM 6 
#define WIN_HEIGHT 800
#define WIN_WIDTH 1024
#define OffsetPiece 5
#define LINEWIDTH 3

static void Init_Initial_Window();
static void draw_line(GtkWidget *widget, gdouble basex, gdouble basey,gdouble endx, gdouble endy, int size);
static void draw_circle(GtkWidget *widget, gdouble basex, gdouble basey, gint radio,int size);
static void Render(GtkWidget *widget);
static void funcCreate();
void Init_Output();
static void ScrollUp();
static void ScrollDown();
static void SaveFuntion();
static void save_to_file (gchar *filename);
static void closerpoint(gint pointerx,gint pointery);

GtkWidget *window,*prewindow;
GtkWidget *fixed,*fixed2;
GtkWidget *label,*lblPer;
GtkLabel *label2,*lblPer2;
GtkWidget *vbox;

GtkWidget *menubar;
GtkWidget *filemenu;
GtkWidget *file;
GtkWidget *newfile;
GtkWidget *separator;
GtkWidget *savefile;
GtkWidget *quit;

GtkWidget *drawingArea2;
GtkDrawingArea *drawingArea;
GdkPixmap *pixMap;	

GtkWidget *txtPer;
GtkEntry *txtPer2;
GtkTextView *txtStart2,*txtProgram2,*txtEnd2;

GtkTextBuffer *buffer;
GtkTextIter start, end;
GtkTextIter iter;
GtkWidget *view;

GtkWidget *lblsize,*lblspeed,*lblcut,*lblunits,*lblplane,*lblwid,*lblhei;
GtkLabel *lblsize2,*lblspeed2,*lblcut2,*lblunits2,*lblplane2,*lblwid2,*lblhei2;
GtkWidget *create,*cancel;

GtkWidget *combo,*combo2,*combo3;
GtkWidget *txtwid,*txthei,*txtspeed;
GtkEntry *txtwid2,*txthei2,*txtspeed2;
gchar *tline1,*tline2,*tline3,*tline4,*tline5,*tline6;

GtkWidget *output,*fixed3;
GtkTextBuffer *buffer2;
GtkTextIter start2, end2;
GtkTextIter iter2;
GtkWidget *view2;
GtkWidget *sup,*sdown;
gint downscroll=0;



int PartWidth=80;
int PartHeight=50;
char StartProgram[200];
char EndProgram[20];
char Program[12000];
gboolean Clicked;
int cx1,cx2,cy1,cy2;
int rx1,rx2,ry1,ry2;
int totalW,totalH;
int PointTemp=-1;

gchar *totalprogram;

typedef struct{
	int x,y;
}Coordenade;

typedef struct{
	int x1,y1,x2,y2;
}Line;

Coordenade Points[300];
Line Lines[100];

int PointCounter=0;
int LineCounter=0;


unsigned int i;
int main(int argc,char *argv[])
{
    totalprogram= g_strdup_printf("");
	gtk_init (&argc, &argv);
	Init_Initial_Window();
	Init_Output();
	Init_GUI();
    gtk_main ();
    
    return 0;
}
static void Init_Initial_Window(){
	prewindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(prewindow), "New Program...");
	gtk_window_set_default_size(GTK_WINDOW(prewindow), 600,280);
	gtk_window_set_position(GTK_WINDOW(prewindow), GTK_WIN_POS_CENTER);
	fixed2 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(prewindow), fixed2);
	
	lblsize = gtk_label_new("Piece Size");
	lblsize2=(GtkLabel*)lblsize;
	gtk_fixed_put(GTK_FIXED(fixed2), lblsize, 30, 30);	
	
	lblplane = gtk_label_new("Plane");
	lblplane2=(GtkLabel*)lblplane;
	gtk_fixed_put(GTK_FIXED(fixed2), lblplane, 200, 30);
	
	lblunits = gtk_label_new("Units");
	lblunits2=(GtkLabel*)lblunits;
	gtk_fixed_put(GTK_FIXED(fixed2), lblunits, 350, 30);
	
	lblspeed= gtk_label_new("Spindle Speed");
	lblspeed2=(GtkLabel*)lblspeed;
	gtk_fixed_put(GTK_FIXED(fixed2), lblspeed, 30, 140);
	
	lblcut = gtk_label_new("Spindle Cut");
	lblcut2=(GtkLabel*)lblcut;
	gtk_fixed_put(GTK_FIXED(fixed2), lblcut, 200, 140);
	
	lblwid = gtk_label_new("Width");
	lblwid2=(GtkLabel*)lblwid;
	gtk_fixed_put(GTK_FIXED(fixed2), lblwid, 20, 60);
	
	lblhei = gtk_label_new("Height");
	lblhei2=(GtkLabel*)lblhei;
	gtk_fixed_put(GTK_FIXED(fixed2), lblhei, 20, 100);
	
	create = gtk_button_new_with_label("Create");
	cancel = gtk_button_new_with_label("Cancel");

	gtk_fixed_put(GTK_FIXED(fixed2), create, 180, 220);
	gtk_widget_set_size_request(create, 80, 25);
	
	gtk_fixed_put(GTK_FIXED(fixed2), cancel, 280, 220);
	gtk_widget_set_size_request(cancel, 80, 25);
	
	
	combo = gtk_combo_box_new_text();
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "XY Plane");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "XZ Plane");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "YZ Plane");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),0);
	gtk_fixed_put(GTK_FIXED(fixed2), combo, 220, 50);
	
	combo2 = gtk_combo_box_new_text();
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo2), "Medium");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo2), "In");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo2), "Out");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo2),0);
	gtk_fixed_put(GTK_FIXED(fixed2), combo2, 220, 170);
	
	combo3 = gtk_combo_box_new_text();
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3), "Milimeters");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3), "Inches");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo3),0);
	gtk_fixed_put(GTK_FIXED(fixed2), combo3, 370, 50);

	txtwid= gtk_entry_new();
	txtwid2=(GtkEntry*)txtwid;
	gtk_entry_set_text(txtwid2,"80");
	gtk_fixed_put(GTK_FIXED(fixed2), txtwid, 80 , 60);
	gtk_widget_set_size_request(txtwid, 50, 25);
	
	txthei= gtk_entry_new();
	txthei2=(GtkEntry*)txthei;
	gtk_entry_set_text(txthei2,"50");
	gtk_fixed_put(GTK_FIXED(fixed2), txthei, 80 ,100);
	gtk_widget_set_size_request(txthei, 50, 25);
	
	txtspeed= gtk_entry_new();
	txtspeed2=(GtkEntry*)txtspeed;
	gtk_entry_set_text(txtspeed2,"2000");
	gtk_fixed_put(GTK_FIXED(fixed2), txtspeed, 50 ,170);
	gtk_widget_set_size_request(txtspeed, 80, 25);
	
	
	g_signal_connect(G_OBJECT(create), "clicked",G_CALLBACK(funcCreate), NULL);
	g_signal_connect(G_OBJECT(cancel), "clicked",G_CALLBACK(gtk_main_quit), G_OBJECT(window));
	
	g_signal_connect_swapped(G_OBJECT(prewindow), "destroy",G_CALLBACK(gtk_main_quit), NULL);

	
	gtk_widget_show_all(prewindow);
}

static void funcCreate()
{ 
	gchar *tplane;
	switch(gtk_combo_box_get_active(GTK_COMBO_BOX(combo))){
		case 0:
			tplane= g_strdup_printf("G17 ");
			break;
		case 1:
			tplane= g_strdup_printf("G18 ");
			break;
		case 2:
			tplane= g_strdup_printf("G19 ");
			break;	
	}
	
	gchar *tunits;
	switch(gtk_combo_box_get_active(GTK_COMBO_BOX(combo3))){
		case 0:
			tunits= g_strdup_printf("M71 ");
			break;
		case 1:
			tunits= g_strdup_printf("M72 ");
			break;
	}
	
	tline1 = g_strdup_printf("%sG90 G54 %s\n",tplane,tunits);
	tline2 = g_strdup_printf("T05 M06\n",tplane,tunits);
	tline3 = g_strdup_printf("S%s\n",gtk_entry_get_text(txtspeed2));


	switch(gtk_combo_box_get_active(GTK_COMBO_BOX(combo))){
		case 0:
			tline4 = g_strdup_printf("G40 G43\n");
			break;
		case 1:
			tline4 = g_strdup_printf("G41 G43\n");
			break;
		case 2:
			tline4 = g_strdup_printf("G42 G43\n");
			break;	
	}
	tline5 = g_strdup_printf("G00 X0 Y0 Z1 F100\n");
	
	gtk_text_buffer_insert(buffer, &iter, tline1, -1);
	gtk_text_buffer_insert(buffer, &iter, tline2, -1);
	gtk_text_buffer_insert(buffer, &iter, tline3, -1);
	gtk_text_buffer_insert(buffer, &iter, tline4, -1);
	
	totalW=(PartWidth + (OffsetPiece * 2))*ZOOM;
	totalH=(PartHeight + (OffsetPiece * 2))*ZOOM;

	gtk_drawing_area_size(drawingArea, totalW,totalH);
	
	gtk_widget_hide_all(prewindow);
	gtk_widget_show_all(window);
	gtk_widget_show_all(output);
}

void Init_Output(){
	output = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(output), "Program Output");
	gtk_window_set_default_size(GTK_WINDOW(output), 300,300);
	//gtk_window_set_position(GTK_WINDOW(output), GTK_WIN_POS_CENTER);
	
	fixed3 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(output), fixed3);
	
	sup = gtk_button_new_with_label("Up");
	sdown = gtk_button_new_with_label("Down");
	
	gtk_fixed_put(GTK_FIXED(fixed3), sup, 250, 100);
	gtk_widget_set_size_request(sup, 45, 45);
	
	gtk_fixed_put(GTK_FIXED(fixed3), sdown, 250, 200);
	gtk_widget_set_size_request(sdown, 45, 45);
	
	
	//////TextView
	
	
	view = gtk_text_view_new();
	gtk_fixed_put(GTK_FIXED(fixed3), view,10, 10);
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	
	gtk_text_buffer_create_tag(buffer, "gap",
							   "pixels_above_lines", 50, NULL);
	
	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
	
	view2 = gtk_text_view_new();
	gtk_fixed_put(GTK_FIXED(fixed3), view2,10, 80);
	
	buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view2));
	
	gtk_text_buffer_create_tag(buffer2, "gap",
							   "pixels_above_lines", 500, NULL);
	
	gtk_text_buffer_get_iter_at_offset(buffer2, &iter2, 0);
	
	
	g_signal_connect(G_OBJECT(sup), "clicked",G_CALLBACK(ScrollUp), NULL);
	g_signal_connect(G_OBJECT(sdown), "clicked",G_CALLBACK(ScrollDown), NULL);
	//gtk_text_buffer_insert(buffer, &iter, "Plain text  \n", -1);
	/*gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, 
	 "Colored Text\n", -1, "blue_fg", "lmarg",  NULL);
	 gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
	 "Text with colored background\n", -1, "lmarg", "gray_bg", NULL);
	 
	 gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
	 "Text in italics\n", -1, "italic", "lmarg",  NULL);
	 
	 gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
	 "Bold text\n", -1, "bold", "lmarg",  NULL);*/
    
	
	/////	
}

static void ScrollUp(){
	downscroll+=10;
	gtk_fixed_move(GTK_FIXED(fixed3), view,10, 10-downscroll);
	gtk_fixed_move(GTK_FIXED(fixed3), view2,10, 80-downscroll);
}
static void ScrollDown(){
	downscroll-=10;
	gtk_fixed_move(GTK_FIXED(fixed3), view,10, 10-downscroll);
	gtk_fixed_move(GTK_FIXED(fixed3), view2,10, 80-downscroll);
}

void Init_GUI()
{
  
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), WIN_TITLE);
	gtk_window_set_default_size(GTK_WINDOW(window), WIN_WIDTH,WIN_HEIGHT);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);
	
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_fixed_put(GTK_FIXED(fixed), vbox, 0, 0);	
	gtk_widget_set_size_request(vbox, WIN_WIDTH,WIN_HEIGHT);
	
	menubar = gtk_menu_bar_new();
	filemenu = gtk_menu_new();
	
	file = gtk_menu_item_new_with_mnemonic("_File");
	newfile = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
	quit = gtk_menu_item_new_with_label("Quit");
	separator=gtk_separator_menu_item_new();
	savefile = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), newfile);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), savefile);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), separator);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 3);
	
	g_signal_connect(G_OBJECT(quit), "activate",G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(savefile), "activate",G_CALLBACK(SaveFuntion), NULL);
	
	
	drawingArea2 = gtk_drawing_area_new();
	
    gtk_signal_connect (GTK_OBJECT (drawingArea2), "expose_event",
						(GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT(drawingArea2),"configure_event",
						(GtkSignalFunc) configure_event, NULL);
    gtk_signal_connect (GTK_OBJECT (drawingArea2), "motion_notify_event",
						(GtkSignalFunc) motion_notify_event, NULL);
    gtk_signal_connect (GTK_OBJECT (drawingArea2), "button_press_event",
						(GtkSignalFunc) button_press_event, NULL);
	
    gtk_widget_set_events (drawingArea2, GDK_EXPOSURE_MASK
						   | GDK_LEAVE_NOTIFY_MASK
						   | GDK_BUTTON_PRESS_MASK
						   | GDK_POINTER_MOTION_MASK
						   | GDK_POINTER_MOTION_HINT_MASK);
	
    
	totalW=(PartWidth + (OffsetPiece * 2))*ZOOM;
	totalH=(PartHeight + (OffsetPiece * 2))*ZOOM;
	
	drawingArea = (GtkDrawingArea*)drawingArea2;
	gtk_drawing_area_size(drawingArea, totalW,totalH);
	gtk_fixed_put(GTK_FIXED(fixed), drawingArea2, 10, 35);	
	
	label = gtk_label_new("X: __  Y: __");
	label2=(GtkLabel*)label;
	gtk_fixed_put(GTK_FIXED(fixed), label, 20, totalH+50);	
	
	lblPer = gtk_label_new("Drilling");
	lblPer2=(GtkLabel*)lblPer2;
	gtk_fixed_put(GTK_FIXED(fixed), lblPer, 120, totalH+50);
	
	txtPer= gtk_entry_new();
	txtPer2=(GtkEntry*)txtPer;
	gtk_entry_set_text(txtPer2,"5");
	gtk_fixed_put(GTK_FIXED(fixed), txtPer, 180 , totalH+50);
	
		
	
	
	g_signal_connect_swapped(G_OBJECT(window), "destroy", 
      G_CALLBACK(gtk_main_quit), NULL);

	//gtk_widget_show_all(window);
}

static void draw_circle(GtkWidget *widget, gint basex, gint basey, gint radio,int size){
	gdk_draw_arc(pixMap,widget->style->black_gc,FALSE,basex-radio,basey-radio,radio*2,radio*2,0,45*64);
	
}

static void draw_line(GtkWidget *widget, gint basex, gint basey,gint endx, gint endy, int size){
	int low = (size >> 1) - size+1;
	int high = (size >> 1)+1;
	int i, j;
	int x1, x2, y1, y2;
	
	for (i = low; i < high; i++){
		for (j = low; j < high; j++){
			if ((basex+i >= 0) && (basey+j >= 0)&&(basex+i < totalW) && (basey+j < totalH)){
				//PA_Put8bitPixel(screen, basex+i, basey+j, color);
				//ML_SetPixelColor(image, basex+i, basey+j, color);
				//SetPixelToBuffer(image, basex+i, basey+j, color);
				gdk_draw_line(pixMap,widget->style->black_gc,x1,y1,x1,y1);
			}
		}
	}
	
	
	for (i = low; i < high; i++){
		j = low;
		x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
		while(x1 < 0) x1++;	while(x1 > totalW) x1--;
		while(x2 < 0) x2++;	while(x2 > totalW) x2--;
		while(y1 < 0) y1++;	while(y1 > totalH) y1--;
		while(y2 < 0) y2++;	while(y2 > totalH) y2--;		
		//PA_Draw8bitLine(image, x1, y1, x2, y2, color);
		gdk_draw_line(pixMap,widget->style->black_gc,x1,y1,x2,y2);
		
		j = high-1;
		x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
		while(x1 < 0) x1++;	while(x1 > totalW) x1--;
		while(x2 < 0) x2++;	while(x2 > totalW) x2--;
		while(y1 < 0) y1++;	while(y1 > totalH) y1--;
		while(y2 < 0) y2++;	while(y2 > totalH) y2--;
		//PA_Draw8bitLine(image, x1, y1, x2, y2, color);
		gdk_draw_line(pixMap,widget->style->black_gc,x1,y1,x2,y2);
	}
	
	for (j = low; j < high; j++){
		i = low;
		x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
		while(x1 < 0) x1++;	while(x1 > totalW) x1--;
		while(x2 < 0) x2++;	while(x2 > totalW) x2--;
		while(y1 < 0) y1++;	while(y1 > totalH) y1--;
		while(y2 < 0) y2++;	while(y2 > totalH) y2--;	
		//PA_Draw8bitLine(image, x1, y1, x2, y2, color);
		gdk_draw_line(pixMap,widget->style->black_gc,x1,y1,x2,y2);
		i = high-1;
		x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
		while(x1 < 0) x1++;	while(x1 > totalW) x1--;
		while(x2 < 0) x2++;	while(x2 > totalW) x2--;
		while(y1 < 0) y1++;	while(y1 > totalH) y1--;
		while(y2 < 0) y2++;	while(y2 > totalH) y2--;	
		gdk_draw_line(pixMap,widget->style->black_gc,x1,y1,x2,y2);
		//PA_Draw8bitLine(image, x1, y1, x2, y2, color);		
	}
	
}
static void draw_brush (GtkWidget *widget, gint x, gint y)
{
	GdkRectangle update_rect;
	
	update_rect.x = x - 5;
	update_rect.y = y - 5;
	update_rect.width = 5;
	update_rect.height = 5;
	gdk_draw_rectangle (pixMap,
						widget->style->black_gc,
						TRUE,
						update_rect.x, update_rect.y,
						update_rect.width, update_rect.height);
	gtk_widget_queue_draw_area (widget,
								update_rect.x, update_rect.y,
								update_rect.width, update_rect.height);
}

/* Redraw the screen from the backing pixmap */
static gboolean expose_event( GtkWidget *widget, GdkEventExpose *event )
{
	gdk_draw_drawable(widget->window,
					  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
					  pixMap,
					  event->area.x, event->area.y,
					  event->area.x, event->area.y,
					  event->area.width, event->area.height);
	
	return FALSE;
}
/* Create a new backing pixmap of the appropriate size */
static gboolean configure_event( GtkWidget *widget, GdkEventConfigure *event )
{
	if (pixMap)
		g_object_unref(pixMap);
	
	pixMap = gdk_pixmap_new(widget->window,
							widget->allocation.width,
							widget->allocation.height,
							-1);
	
	gdk_draw_rectangle (pixMap,
						widget->style->white_gc,
						TRUE,
						0, 0,
						widget->allocation.width,
						widget->allocation.height);
	
	Render(widget);
	return TRUE;
}

static gboolean button_press_event( GtkWidget *widget, GdkEventButton *event )
{
	if (event->button == 1 && pixMap != NULL){
		if (Clicked==FALSE){
			
			cx1=(gint)(event->x/ZOOM)-OffsetPiece;
			cy1=(gint)((event->y/ZOOM)-OffsetPiece-PartHeight)*-1;
			rx1=(gint)(cx1+OffsetPiece)*ZOOM;
			ry1=(gint)((cy1*-1)+OffsetPiece+PartHeight)*ZOOM;
			Clicked=TRUE;
			draw_circle(widget, rx1, ry1, 20,20);
		}else{
			cx2=(gint)(event->x/ZOOM)-OffsetPiece;
			cy2=(gint)((event->y/ZOOM)-OffsetPiece- PartHeight)*-1;
			rx2=(cx2+OffsetPiece)*ZOOM;
			ry2=((cy2*-1)+OffsetPiece+PartHeight)*ZOOM;
			Clicked=TRUE;
			draw_line (widget, rx1, ry1,rx2, ry2,LINEWIDTH);
			
			gchar *tempcommand = g_strdup_printf("G00 X%d Y%d Z3 F100\nG01 Z-%s\nG01 X%d Y%d Z-%s\nG00 Z3\n", cx1,cy1,gtk_entry_get_text(txtPer2),cx2,cy2,gtk_entry_get_text(txtPer2));
			
			totalprogram= g_strdup_printf("%s%s",totalprogram,tempcommand);
			
			gtk_text_buffer_insert(buffer, &iter, tempcommand, -1);
			
			
			Points[PointCounter].x=rx1; Points[PointCounter].y=ry1;
			PointCounter++;
			Points[PointCounter].x=rx2; Points[PointCounter].y=ry2;
			PointCounter++;
			
			Lines[LineCounter].x1=cx1; Lines[LineCounter].y1=cy1;
			Lines[LineCounter].x2=cx2; Lines[LineCounter].y2=cy2;
			LineCounter++;
			Render(widget);
			Clicked=FALSE;
		}
	}
	
	return TRUE;
}

static gboolean motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
	int x, y;
	GdkModifierType state;
	
	//if (event->is_hint)
		//gdk_window_get_pointer (event->window, &x, &y, &state);
	//else
    ///*{
		x = (gint)event->x;
		y = (gint)event->y;
		//state = event->state;
    //}*/
    int tx,ty;
    tx=(gint)(event->x/ZOOM)-OffsetPiece;
	ty=(gint)((event->y/ZOOM)-OffsetPiece-PartHeight)*-1;
	closerpoint(x,y);
	
	
	if(Clicked==TRUE){
		Render(widget);
		draw_line (widget, rx1, ry1,(gint)event->x, (gint)event->y,LINEWIDTH);
		
	}

	gchar *tempchar;
	tempchar = g_strdup_printf("X: %02d Y: %02d",(gint) (event->x/ZOOM)-OffsetPiece,(gint) ((event->y/ZOOM)-OffsetPiece- PartHeight)*-1);
	
	gtk_label_set_text(label2,tempchar);
	
	/*
	if (state & GDK_BUTTON1_MASK && pixMap != NULL)
		draw_brush (widget, x, y);*/
	
	return TRUE;
}
static void closerpoint(gint pointerx,gint pointery){
    int i,temp1,temp2;
    PointTemp=-1;
    for(i=0;i<PointCounter; i++){
        temp1=Points[PointCounter].x-pointerx; 
        temp2=Points[PointCounter].y-pointery;
        if((temp1>=-20&&temp1<=20)&&(temp2>=-20&&temp2<=20)){
           PointTemp=i;
           break;
        }
    }
       
}
static void Render(GtkWidget *widget){
	gdk_draw_rectangle (pixMap,
						widget->style->white_gc,
						TRUE,
						0, 0,totalW,totalH);
	int i;
	int tempx1,tempx2,tempy1,tempy2;
	
	
	tempx1=(0+OffsetPiece)*ZOOM;
	tempy1=((0*-1)+OffsetPiece+PartHeight)*ZOOM;
	tempx2=(PartWidth+OffsetPiece)*ZOOM;
	tempy2=((0*-1)+OffsetPiece+PartHeight)*ZOOM;
	draw_line (widget, tempx1, tempy1,tempx2, tempy2,1);
	
	tempx1=(PartWidth+OffsetPiece)*ZOOM;
	tempy1=((0*-1)+OffsetPiece+PartHeight)*ZOOM;
	tempx2=(PartWidth+OffsetPiece)*ZOOM;
	tempy2=((PartHeight*-1)+OffsetPiece+PartHeight)*ZOOM;
	draw_line (widget, tempx1, tempy1,tempx2, tempy2,1);
	
	tempx1=(0+OffsetPiece)*ZOOM;
	tempy1=((PartHeight*-1)+OffsetPiece+PartHeight)*ZOOM;
	tempx2=(PartWidth+OffsetPiece)*ZOOM;
	tempy2=((PartHeight*-1)+OffsetPiece+PartHeight)*ZOOM;
	draw_line (widget, tempx1, tempy1,tempx2, tempy2,1);
	
	tempx1=(0+OffsetPiece)*ZOOM;
	tempy1=((0*-1)+OffsetPiece+PartHeight)*ZOOM;
	tempx2=(0+OffsetPiece)*ZOOM;
	tempy2=((PartHeight*-1)+OffsetPiece+PartHeight)*ZOOM;
	draw_line (widget, tempx1, tempy1,tempx2, tempy2,1);
	
	for(i=0;i<LineCounter; i++){
		
		tempx1=(Lines[i].x1+OffsetPiece)*ZOOM;
		tempy1=((Lines[i].y1*-1)+OffsetPiece+PartHeight)*ZOOM;
		tempx2=(Lines[i].x2+OffsetPiece)*ZOOM;
		tempy2=((Lines[i].y2*-1)+OffsetPiece+PartHeight)*ZOOM;
		draw_line (widget, tempx1, tempy1,tempx2, tempy2,LINEWIDTH);
	}
	if(PointTemp>=0)
	  draw_circle(widget, Points[PointTemp].x, Points[PointTemp].y, 80,4);

	int tempw = tempx1-tempx2;
	if(tempw<0)tempw*-1;
	int temph = tempy1-tempy2;
	if(temph<0)temph*-1;
	
	//gtk_widget_queue_draw_area (widget,tempx1, tempy1,tempw,temph);
	gtk_widget_queue_draw_area (widget,0, 0,(PartWidth + (OffsetPiece * 2))*ZOOM, (PartHeight + (OffsetPiece * 2))*ZOOM);
	
}

static void SaveFuntion(){
	GtkWidget *dialog;
	
	dialog = gtk_file_chooser_dialog_new ("Save File",
										  GTK_WINDOW(window),
										  GTK_FILE_CHOOSER_ACTION_SAVE,
										  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
										  NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
	
	//if (user_edited_a_new_document)
	//{
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "");
		gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled Program.txt");
	//}
	//else
		//gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		save_to_file (filename);
		g_free (filename);
	}
	
	gtk_widget_destroy (dialog);
	
}

static void save_to_file (gchar *filename){
	
	ofstream file;
	file.open(filename);
	file << tline1<<tline2<<tline3<<tline4<<tline5<<totalprogram<<"M30\nG28";
	file.close();
	
	gchar *command = g_strdup_printf("notepad %s",filename);
	
	system(command);
	
	/*FILE *file;
	
	if ( (file=fopen(filename,"wb")) == NULL)
	{  
		printf("nopppp");
	}
	else fwrite(totalprogram, sizeof(totalprogram)*3.5, sizeof(totalprogram), file);
	fclose(file);*/
	
}

