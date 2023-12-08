#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>
#include <gtk/gtktable.h>
#include <gtk/gtktogglebutton.h>
#include <gtk/gtksignal.h>
#include "game_2048.h"

//#define DEBUG
#ifdef DEBUG
#define TG2048_DBG g_print
#else
#define TG2048_DBG
#endif

#define TESTER_SUCCEEDING (1)
#define TESTER_FAILING (-1)
//#define TESTER TESTER_SUCCEEDING

//header part begin
G_BEGIN_DECLS

#define TG2048_TYPE            (tg2048_get_type())
#define TG2048(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), TG2048_TYPE, Tg2048))
#define TG2048_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), TG2048_TYPE, Tg2048Class))
#define IS_TG2048(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), TG2048_TYPE))
#define IS_TG2048_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TG2048_TYPE))

typedef struct _Tg2048       Tg2048;
typedef struct _Tg2048Class  Tg2048Class;

struct _Tg2048
{
	GtkTable table;

	GtkWidget *buttons[4][4];
	gulong handler_id[4];
};

struct _Tg2048Class
{
	GtkTableClass parent_class;

	void (* tg2048_dft_hdl) (Tg2048 *tg);
};

static GType          tg2048_get_type        (void);
static GtkWidget*     tg2048_new             (void);

G_END_DECLS
//header part end

//implementation begin
enum {
	TG2048_MOVE_UP_SIGNAL,
	TG2048_MOVE_DOWN_SIGNAL,
	TG2048_MOVE_LEFT_SIGNAL,
	TG2048_MOVE_RIGHT_SIGNAL,
	TG2048_LAST_SIGNAL
};

static guint tg2048_signals[TG2048_LAST_SIGNAL] = { 0 };
static int tg2048_last_released_row = -1;
static int tg2048_last_released_column = -1;

static void tg2048_class_init (Tg2048Class*);
static void tg2048_init       (Tg2048*);
static void tg2048_released   (GtkWidget*, Tg2048*);
static void tg2048_enter      (GtkWidget*, Tg2048*);
static int set_value          (Tg2048*, int**);
static int get_index          (Tg2048*, GtkWidget*, int*, int*);

static GType tg2048_get_type(void)
{
	static GType tg2048_type = 0;

	if(!tg2048_type)
	{
		static const GTypeInfo tg_info =
		{
			sizeof(Tg2048Class),
			NULL, /* base_init */
			NULL, /* base_finalize */
			(GClassInitFunc)tg2048_class_init,
			NULL, /* class_finalize */
			NULL, /* class_data */
			sizeof(Tg2048),
			0,
			(GInstanceInitFunc)tg2048_init,
		};

		tg2048_type = g_type_register_static(GTK_TYPE_TABLE, "Tg2048", &tg_info, 0);
	}

	return tg2048_type;
}

static void tg2048_init(Tg2048* tg)
{
	gint i = 0;
	gint j = 0;

	gtk_table_resize(GTK_TABLE(tg), 4, 4);
	gtk_table_set_homogeneous(GTK_TABLE(tg), TRUE);

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			tg->buttons[i][j] = gtk_button_new();
			gtk_table_attach_defaults(GTK_TABLE(tg), tg->buttons[i][j],
				j, j+1, i, i+1); //row and column should be taken care
			g_signal_connect(G_OBJECT(tg->buttons[i][j]), "released",
				G_CALLBACK(tg2048_released), (gpointer)tg);
			g_signal_connect(G_OBJECT(tg->buttons[i][j]), "enter",
				G_CALLBACK(tg2048_enter), (gpointer)tg);
			gtk_widget_set_size_request(tg->buttons[i][j], 80, 80);
			gtk_widget_show(tg->buttons[i][j]);
		}
	}
}

static void tg2048_class_init(Tg2048Class* klass)
{
	tg2048_signals[TG2048_MOVE_UP_SIGNAL] = g_signal_new("move_up",
		G_TYPE_FROM_CLASS(klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		G_STRUCT_OFFSET(Tg2048Class, tg2048_dft_hdl),
		NULL,
		NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
	tg2048_signals[TG2048_MOVE_DOWN_SIGNAL] = g_signal_new("move_down",
		G_TYPE_FROM_CLASS(klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		G_STRUCT_OFFSET(Tg2048Class, tg2048_dft_hdl),
		NULL,
		NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
	tg2048_signals[TG2048_MOVE_LEFT_SIGNAL] = g_signal_new("move_left",
		G_TYPE_FROM_CLASS(klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		G_STRUCT_OFFSET(Tg2048Class, tg2048_dft_hdl),
		NULL,
		NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
	tg2048_signals[TG2048_MOVE_RIGHT_SIGNAL] = g_signal_new("move_right",
		G_TYPE_FROM_CLASS(klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		G_STRUCT_OFFSET(Tg2048Class, tg2048_dft_hdl),
		NULL,
		NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
}

GtkWidget* tg2048_new(void)
{
	return GTK_WIDGET(g_object_new(tg2048_get_type(), NULL));
}

static int set_value(Tg2048* tg, int** val)
{
	gint i = 0;
	gint j = 0;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			gchar num[5] = {'\0'};
			if(0<val[i][j])
			{
				snprintf(num, sizeof(num), "%d", val[i][j]);
			}
			gtk_button_set_label((GtkButton *)G_OBJECT(tg->buttons[i][j]), (const gchar*)num);
		}
	}
	return 0;
}

static int get_index(Tg2048* tg, GtkWidget *widget, int* r, int* c)
{
	gint i = 0;
	gint j = 0;
	*r = -1;
	*c = -1;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			if(tg->buttons[i][j] == widget)
			{
				*r = i;
				*c = j;
				return 0;
			}
		}
	}
	return -1;
}

static void tg2048_released(GtkWidget* widget, Tg2048* tg)
{
	int r = 0;
	int c = 0;
	int ret = get_index(tg, widget, &r, &c);
	if(ret == 0)
	{
		TG2048_DBG("%s; row = %d, column = %d.\n", __FUNCTION__, r, c);
		tg2048_last_released_row = r;
		tg2048_last_released_column = c;
	}
}

static void tg2048_enter(GtkWidget* widget, Tg2048* tg)
{
	int r = 0;
	int c = 0;
	int ret = get_index(tg, widget, &r, &c);
	if(ret == 0)
	{
		TG2048_DBG("%s; row = %d, column = %d.\n", __FUNCTION__, r, c);
		if(tg2048_last_released_row != -1 && tg2048_last_released_column != -1)
		{
			if(tg2048_last_released_row != r || tg2048_last_released_column != c)
			{
				if(tg2048_last_released_row != r)
				{
					if(tg2048_last_released_row > r)
					{
						TG2048_DBG("%s; go up\n", __FUNCTION__);
						g_signal_emit(G_OBJECT(tg), tg2048_signals[TG2048_MOVE_UP_SIGNAL], 0);
					}
					else
					{
						TG2048_DBG("%s; go down\n", __FUNCTION__);
						g_signal_emit(G_OBJECT(tg), tg2048_signals[TG2048_MOVE_DOWN_SIGNAL], 0);
					}
				}
				else
				{
					if(tg2048_last_released_column > c)
					{
						TG2048_DBG("%s; go left\n", __FUNCTION__);
						g_signal_emit(G_OBJECT(tg), tg2048_signals[TG2048_MOVE_LEFT_SIGNAL], 0);
					}
					else
					{
						TG2048_DBG("%s; go right\n", __FUNCTION__);
						g_signal_emit(G_OBJECT(tg), tg2048_signals[TG2048_MOVE_RIGHT_SIGNAL], 0);
					}
				}
			}
			tg2048_last_released_row = -1;
			tg2048_last_released_column = -1;
		}
	}
}
//implementation end

//tester begin
static int** mat = NULL;
static int changed = 0;
static game_2048_status status = GAME_2048_STATUS_NOT_OVER;
static GtkWidget* window = NULL;

static void pattern_succeeding(int** mat) __attribute__((unused));
static void pattern_failing(int** mat) __attribute__((unused));

static void freeze(Tg2048* tg)
{
	gint i = 0;
	for(i=0; i<(sizeof(tg->handler_id)/sizeof(tg->handler_id[0])); i++)
	{
		g_signal_handler_block(G_OBJECT(tg), tg->handler_id[i]);
	}
}

static void pop_up(const char* msg)
{
	GtkWidget* dialog = NULL;

	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_OTHER,
		GTK_BUTTONS_OK,
		"Result: ");
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", msg);
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

static void move_up(GtkWidget* widget, gpointer data)
{
	game_2048_move_up(&mat, &changed);
	game_2048_get_status(mat, &status);
	if(GAME_2048_STATUS_NOT_OVER == status)
	{
		game_2048_add_new_2(mat);
	}
	set_value((Tg2048*)widget, mat);
	if(GAME_2048_STATUS_NOT_OVER != status)
	{
		TG2048_DBG("%s; result = %s\n", __FUNCTION__, (GAME_2048_STATUS_WON == status)?"Won":"Lost");
		pop_up((GAME_2048_STATUS_WON == status)?"Won":"Lost");
		freeze((Tg2048*)widget);
	}
}

static void move_down(GtkWidget* widget, gpointer data)
{
	game_2048_move_down(&mat, &changed);
	game_2048_get_status(mat, &status);
	if(GAME_2048_STATUS_NOT_OVER == status)
	{
		game_2048_add_new_2(mat);
	}
	set_value((Tg2048*)widget, mat);
	if(GAME_2048_STATUS_NOT_OVER != status)
	{
		TG2048_DBG("%s; result = %s\n", __FUNCTION__, (GAME_2048_STATUS_WON == status)?"Won":"Lost");
		pop_up((GAME_2048_STATUS_WON == status)?"Won":"Lost");
		freeze((Tg2048*)widget);
	}
}

static void move_left(GtkWidget* widget, gpointer data)
{
	game_2048_move_left(&mat, &changed);
	game_2048_get_status(mat, &status);
	if(GAME_2048_STATUS_NOT_OVER == status)
	{
		game_2048_add_new_2(mat);
	}
	set_value((Tg2048*)widget, mat);
	if(GAME_2048_STATUS_NOT_OVER != status)
	{
		TG2048_DBG("%s; result = %s\n", __FUNCTION__, (GAME_2048_STATUS_WON == status)?"Won":"Lost");
		pop_up((GAME_2048_STATUS_WON == status)?"Won":"Lost");
		freeze((Tg2048*)widget);
	}
}

static void move_right(GtkWidget* widget, gpointer data)
{
	game_2048_move_right(&mat, &changed);
	game_2048_get_status(mat, &status);
	if(GAME_2048_STATUS_NOT_OVER == status)
	{
		game_2048_add_new_2(mat);
	}
	set_value((Tg2048*)widget, mat);
	if(GAME_2048_STATUS_NOT_OVER != status)
	{
		TG2048_DBG("%s; result = %s\n", __FUNCTION__, (GAME_2048_STATUS_WON == status)?"Won":"Lost");
		pop_up((GAME_2048_STATUS_WON == status)?"Won":"Lost");
		freeze((Tg2048*)widget);
	}
}

static void die(GtkWidget *widget, gpointer data)
{
	game_2048_term(&mat);
	exit(EXIT_SUCCESS);
}

static void pattern_succeeding(int** mat)
{
	mat[0][0] = 1024;
	mat[0][1] = 1024;
	mat[0][2] = 0;
	mat[0][3] = 0;
	mat[1][0] = 1024;
	mat[1][1] = 0;
	mat[1][2] = 0;
	mat[1][3] = 0;
	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 0;
	mat[2][3] = 0;
	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 0;
}

static void pattern_failing(int** mat)
{
	mat[0][0] = 0;
	mat[0][1] = 4;
	mat[0][2] = 8;
	mat[0][3] = 4;
	mat[1][0] = 4;
	mat[1][1] = 8;
	mat[1][2] = 4;
	mat[1][3] = 8;
	mat[2][0] = 8;
	mat[2][1] = 4;
	mat[2][2] = 8;
	mat[2][3] = 4;
	mat[3][0] = 4;
	mat[3][1] = 8;
	mat[3][2] = 4;
	mat[3][3] = 8;
}

static int init(GtkWidget** window, GtkWidget** tg2048)
{
	if(NULL == window || NULL == tg2048)
	{
		return -1;
	}

	*window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(*window), "Tile Game 2048");
	g_signal_connect(G_OBJECT(*window), "destroy", G_CALLBACK(die), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(*window), 10);

	*tg2048 = tg2048_new();

	gtk_container_add(GTK_CONTAINER(*window), *tg2048);
	gtk_widget_show(*tg2048);

	((Tg2048*)(*tg2048))->handler_id[0] = g_signal_connect(G_OBJECT(*tg2048), "move_up", G_CALLBACK(move_up), NULL);
	((Tg2048*)(*tg2048))->handler_id[1] = g_signal_connect(G_OBJECT(*tg2048), "move_down", G_CALLBACK(move_down), NULL);
	((Tg2048*)(*tg2048))->handler_id[2] = g_signal_connect(G_OBJECT(*tg2048), "move_left", G_CALLBACK(move_left), NULL);
	((Tg2048*)(*tg2048))->handler_id[3] = g_signal_connect(G_OBJECT(*tg2048), "move_right", G_CALLBACK(move_right), NULL);
	gtk_widget_show(*window);

	game_2048_init(&mat);

#if defined(TESTER) && (TESTER == TESTER_SUCCEEDING)
	pattern_succeeding(mat);
#elif defined(TESTER) && (TESTER == TESTER_FAILING)
	pattern_failing(mat);
#else
	//nothing
#endif

	game_2048_add_new_2(mat);

	set_value((Tg2048*)(*tg2048), mat);
	return 0;
}

int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget* tg2048 = NULL;
	init(&window, &tg2048);

	gtk_main();

	return EXIT_SUCCESS;
}
//tester end
