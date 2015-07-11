/* ========================================================================== */
/* File: AMStartup.c - Amazing Project startup program
 *
 * Author:      Cabernet: Daniel Lee, John Ling, Pratap Luitel
 * Date:        5/20/14
 *
 * Input:       Number of avatars, difficulty level, hostname of server
 *
 * Command line options: -n nAvatars: the number of avatars in the maze
                         -d difficulty: the difficulty level of the maze
                         -h hostname: the hostname of the server
                         			(stowe.cs.dartmouth.edu and carter.cs.dartmouth.edu)
 *
 * Output:      
 
 * Error Conditions:    
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>               // fprintf, fopen...
#include <stdlib.h>              // dynamic memory management
#include <getopt.h>              // getopt_long


#include <netinet/in.h>          // socket programming
#include <sys/socket.h>          // socket programming
#include <time.h>                // time functions for the log file
#include <sys/types.h>
#include <arpa/inet.h>


#include <string.h>              // strlen
#include <unistd.h>              // close()
#include <netdb.h>               // gethostbyname()
#include <pthread.h>             // thread functionality

#include <gtk/gtk.h>             // graphics
#include <cairo.h>               // graphics


// ---------------- Local includes  e.g., "file.h"
#include "AMStartup.h"                   // data structure definitions
#include "avatar.h"



// ---------------- Constant definitions
int graphics_width;
int graphics_height;

#define GRID_SCALER    10
// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes


/* ========================================================================== */
int WALLDIM = 3;
int MAZESQUARE = 17;
static char buffer[256];


void destroy (void)
{
    gtk_main_quit ();
}


static gboolean on_draw_event (GtkWidget *widget, cairo_t *cr)
{
    cr = gdk_cairo_create(gtk_widget_get_window(widget));
    int i;
    int j;

    for (i = 0; i < graphics_width; i++)
    {
        for (j = 0; j < graphics_height; j++)
        {
            if (learn_maze[i][j]->visited == 1)
            {
                cairo_set_source_rgb(cr, 50.0/255.0, 205.0/255.0, 50.0/255.0);

                if (learn_maze[i][j]->gNorth == 1)
                {
                    cairo_rectangle(cr, (i)*MAZESQUARE + (i+1)*WALLDIM, (j)*MAZESQUARE + (j)*WALLDIM, MAZESQUARE, WALLDIM);
                }
                if (learn_maze[i][j]->gSouth == 1)
                {
                    cairo_rectangle(cr, (i)*MAZESQUARE + (i+1)*WALLDIM, (j+1)*MAZESQUARE + (j+1)*WALLDIM, MAZESQUARE, WALLDIM);
                }
                if (learn_maze[i][j]->gWest == 1)
                {
                    cairo_rectangle(cr, (i)*MAZESQUARE + (i)*WALLDIM, (j)*MAZESQUARE + (j+1)*WALLDIM, WALLDIM, MAZESQUARE);
                }
                if (learn_maze[i][j]->gEast == 1)
                {
                    cairo_rectangle(cr, (i+1)*MAZESQUARE + (i+1)*WALLDIM, (j)*MAZESQUARE + (j+1)*WALLDIM, WALLDIM, MAZESQUARE);
                }
                cairo_fill(cr);

                //cairo_set_source_rgb(cr, 1, 1, 1);
                cairo_set_source_rgb(cr, 0, 0, 0);
                cairo_rectangle(cr, (i)*MAZESQUARE + (i+1)*WALLDIM, (j)*MAZESQUARE + (j+1)*WALLDIM, MAZESQUARE, MAZESQUARE);
                cairo_fill(cr);

                if (learn_maze[i][j]->occupied == 1)
                {
                    cairo_set_source_rgb(cr, 1, 1, 1);
                    cairo_rectangle(cr, (i)*MAZESQUARE + (i+1)*WALLDIM + (MAZESQUARE/3), (j)*MAZESQUARE + (j+1)*WALLDIM + (MAZESQUARE/3), MAZESQUARE/3, MAZESQUARE/3);
                    cairo_fill(cr);
                }
            }
        }
    }

    cairo_destroy(cr);

    return FALSE;
}

//modified from http://zetcode.com/tutorials/gtktutorial/gtkevents/
static gboolean time_handler(GtkWidget *widget) 
{

    if (gtk_widget_get_window(widget) == NULL)
    {
        return FALSE;
    }

    // Timer for sleeping.
    time_t curtime;
    struct tm *loctime;

    curtime = time(NULL);
    loctime = localtime(&curtime);
    strftime(buffer, 256, "%T", loctime);

    gtk_widget_queue_draw(widget);
    return TRUE;
}


void* mazeDrawer() 
{
    // The window and background information.
    GtkWidget *window;
    GdkRGBA color = {0, 0, 0, 0};

    // Calculate the window size based on the size of the maze.
    int window_width = graphics_width*MAZESQUARE + (graphics_width + 2)*WALLDIM;
    int window_height = graphics_height*MAZESQUARE + (graphics_height + 2)*WALLDIM;

    gtk_init (0, NULL);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), window_width, window_height);
    gtk_widget_override_background_color(window, GTK_STATE_NORMAL, &color);

    gtk_widget_set_app_paintable(window, TRUE);

 
    g_signal_connect(window, "draw", G_CALLBACK (on_draw_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    g_timeout_add(50, (GSourceFunc) time_handler, (gpointer) window);

    gtk_widget_show_all(window);

    time_handler(window);

    gtk_main ();

    return 0;
}


int main(int argc, char* argv[])
{
    globalZeroX = 0;
    globalZeroY = 0;

    if (argc != 7) 
    {
        fprintf(stderr, "You must enter all 3 switches, -n, -d ,-h ");
        exit(1);
    }
    static struct option long_options[] = 
    {
        {"num_avatars", required_argument, 0, 'n' },
        {"maze_difficulty", required_argument, 0, 'd' },
        {"hose_server", required_argument, 0,'h' },
    };

    char c = 0; 

    uint32_t num_avatars = 0;
    uint32_t maze_difficulty = 0;
    char *server_name;

	//check command line arguments
	while ((c = getopt_long(argc, argv, "n:d:h:", long_options, NULL)) != -1)
	{
        switch (c)
        {

        //number of avatars
        case 'n': ; 
            int avatar_arg = 0;
            sscanf(optarg, "%i", &avatar_arg);
            if (avatar_arg >= 2 && avatar_arg <= AM_MAX_AVATAR)
            {
                num_avatars = avatar_arg;
            }
            else
            {
                fprintf(stderr, "error: number of avatars must be between 2 and 10\n");
                exit(1);
            }
            break;

        //maze difficulty
        case 'd': ; 
            int difficulty_arg = 0;
            int conv_res;
            conv_res = sscanf(optarg, "%i", &difficulty_arg);
            if (conv_res != 1)
            {
                fprintf(stderr, "error: difficulty must be between 0 and 9\n");
                exit(1);
            }
            if(difficulty_arg >= 0 && difficulty_arg <= AM_MAX_DIFFICULTY)
            {
                maze_difficulty = difficulty_arg;
            }
            else 
            {
                fprintf(stderr, "error: difficulty must be between 0 and 9\n");
                exit(1);
            }
            break;

        //host server name
        case 'h':
            server_name = optarg;
            break;

            
        default:
            fprintf(stderr, "error: invalid option \'%c\'\n", c);
            fprintf(stderr, "To execute, use options: %s [-n] [-d] [-h]\n", argv[0]);
            exit(1);
        }
    }

    printf("number of avatars is: %d\n", num_avatars);
    printf("maze difficulty is: %d\n", maze_difficulty);
    printf("host server name is: %s\n", server_name);


    //create log file
    char *user_id = getenv("USER");
    char *log_file_name = calloc(200, sizeof(char));
    printf("USER is: %s\n", user_id);
    sprintf(log_file_name, "Amazing_%s_%d_%d.log", user_id, num_avatars, maze_difficulty);

    FILE *logfile;
    logfile = fopen(log_file_name, "w");

    if (logfile == NULL)
    {
        fprintf(stderr, "error: could not generate log file to write to\n");
        exit(1);
    }

    //////////////////////////////////// BEGIN SOCKET STUFF //////////////////////////////////// 

    //socket() function
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        fprintf(stderr, "error: could not make socket connection\n");
        exit(1);
    }

    //create the AM_INIT message
    AM_Message init_message;
    init_message.type = htonl(AM_INIT);
    init_message.init.nAvatars = htonl(num_avatars);
    init_message.init.Difficulty = htonl(maze_difficulty);

    //get IP address
    struct hostent *host;
    host = gethostbyname(server_name);
    if (host == NULL)
    {
        fprintf(stderr, "error: gethostbyname returned error\n");
        exit(1);
    }

    //create socket
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    struct in_addr *addr = (struct in_addr *)host->h_addr_list[0];
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(inet_ntoa(* addr));
    servaddr.sin_port =  htons( atoi(AM_SERVER_PORT) ); //convert to big-endian order
    
    //connect() function
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        fprintf(stderr, "error: could not connect to server\n");
        exit(1);
    }

    //send() funciton to send AM_INIT message
    if (send(sockfd, &init_message, AM_MAX_MESSAGE, 0) == -1)
    {
        fprintf(stderr, "error: AM_INIT message couldn't be sent\n");
        exit(1);
    }

    //receive message
    AM_Message receive_message;
    if (recv(sockfd, &receive_message, sizeof(receive_message), 0) == 0)
    {
        fprintf(stdout, "error: couldn't receive AM_INIT_OK message\n");
        exit(1);
    }

    if (ntohl(receive_message.type) == AM_INIT_FAILED)
    {
        fprintf(stdout, "error: received AM_INIT_FAILED message\n");
        exit(1);
    }

    if (ntohl(receive_message.type) != AM_INIT_OK) 
    {
        fprintf(stdout, "error: did not received an AM_INIT_OK message\n");
        exit(1);
    }


    //get info about maze
    uint32_t height = ntohl(receive_message.init_ok.MazeHeight);
    uint32_t width = ntohl(receive_message.init_ok.MazeWidth);
    uint32_t maze_port = ntohl(receive_message.init_ok.MazePort);

    graphics_width = ntohl(receive_message.init_ok.MazeWidth);
    graphics_height = ntohl(receive_message.init_ok.MazeHeight);

    printf("maze height is: %d\n", height);
    printf("maze width is: %d\n", width);
    printf("maze port is: %d\n", maze_port);

    if( (graphics_width <= 0) || (graphics_height <= 0) )
    {
        fprintf(stderr, "error: got back an invalid maze from server\n");
        exit(1);
    }

    /////////////////////////////// Write basic stuff to logfile ///////////////////////////////

    time_t init_cur_time = time(NULL);
    char *final_cur_time = ctime(&init_cur_time);
    fprintf(logfile, "user: %s | mazeport: %d | date: %s\n", user_id, maze_port, final_cur_time);

    /////////////////////////////// Create learning maze/array ///////////////////////////////

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            learn_maze[i][j] = malloc(sizeof(MazeTile));
            learn_maze[i][j]->northBlocked = 0;
            learn_maze[i][j]->eastBlocked = 0;
            learn_maze[i][j]->southBlocked = 0;
            learn_maze[i][j]->westBlocked = 0;
            learn_maze[i][j]->visited = 0;
            learn_maze[i][j]->deadEnd = 0;
            learn_maze[i][j]->lastDir = 5;                          
            learn_maze[i][j]->lastAvatar = 11;
            learn_maze[i][j]->mazeWidth = width;
            learn_maze[i][j]->mazeHeight = height;
            learn_maze[i][j]->gNorth = 0;
            learn_maze[i][j]->gEast = 0;
            learn_maze[i][j]->gSouth = 0;
            learn_maze[i][j]->gWest = 0;
            learn_maze[i][j]->occupied = 0;
        }
    }
    /////////////////////////////// Create avatar stuff here ///////////////////////////////
    pthread_t avatar_array[AM_MAX_AVATAR];
    int i;
    for (i = 0; i < num_avatars; i++)
    {
        AvatarStruct *new_avatar = calloc(1, sizeof(AvatarStruct));

        new_avatar->id = i;
        new_avatar->total_num_avatars = num_avatars;
        new_avatar->difficulty = maze_difficulty;
        new_avatar->address = addr;
        new_avatar->maze_port = maze_port;
        new_avatar->logfile_name = log_file_name;
        new_avatar->file = logfile;
        new_avatar->moved = 0;
        new_avatar->dir = M_NORTH;

        int avatar_thread = pthread_create(&avatar_array[i], NULL, avatarFunction, new_avatar);

        if (avatar_thread != 0)
        {
            fprintf(stdout, "could not create thread for avatar: %d\n", avatar_thread);
        }
    }

    pthread_t graphics_thread;
    int gr_thread_res = pthread_create(&graphics_thread, NULL, mazeDrawer, NULL);
    if (gr_thread_res != 0) 
    {
        fprintf(stdout, "could not create thread for graphics\n");
        exit(1);
    }

    for (i = 0; i < num_avatars; i++)
    {
        pthread_join(avatar_array[i], NULL);
    }

    printf("\ndone solving maze, freeing memory and closing logfile\n");
    if (logfile)
    {
        int result;
        result = fclose(logfile);
        
        if (result == 0)
        {
            printf("logfile succesfully closed!\n");
        }
    }
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            free(learn_maze[i][j]);
        }
    }
    free(log_file_name);
    
    if (sockfd)
    {
        close(sockfd);
    }
    //Free memory here

    return 0;

}





  
