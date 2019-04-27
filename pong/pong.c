#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>

#include "pong.h"

struct ball the_ball = {
	.y_pos = Y_INIT,
	.x_pos = X_INIT,
	/* 1: down for y, 0: up for y */
	.y_dir = 1,
	/* 1: right for x, 0: left for x */
	.x_dir = 1,

	.symbol = DEL_SYMBOL,
};

static struct window the_window = {
	.up_left_x = 2,
	.up_left_y = 2,
	.down_right_x = 80,
	.down_right_y = 30,
	.gate = {
		.pos = 4,
		.size = GATE_SIZE,
		.window = &the_window,
	},
};

/* drawn gate successful returns 0, other return -1 */
int drawn_gate(struct gate *gate)
{
	int i;
	struct window *window = gate->window;
	/* x position of the door */
	int j = window->down_right_x + 1;

	int up_limit = window->up_left_y;
	int down_limit = window->down_right_y;

	if (gate->pos < up_limit ||
	    gate->pos + gate->size - 1 > down_limit)
		return -1;

	for (i = gate->pos; i <= (gate->pos + gate->size - 1); i++)
		mvaddch(i, j, DEL_LINE);

	return 0;
}

void drawn_window(struct window *window)
{
	int i = window->up_left_y + 1;
	int j = window->up_left_x;

	for (; j <= (window->down_right_x); j++) {
		mvaddch(window->up_left_y,    j, DEL_LINE);
		mvaddch(window->down_right_y, j, DEL_LINE);
	}

	for (; i <= (window->down_right_y - 1); i++)
		mvaddch(i, window->up_left_x, DEL_LINE);

	drawn_gate(&window->gate);
}

/* 1: up, 0: down */
int move_gate(struct gate *gate, bool up)
{
	int pos;
	struct window *window = gate->window;
	int up_limit = window->up_left_y;
	int down_limit = window->down_right_y;
	int x_door = window->down_right_x + 1;

	if (up == true)
		pos = gate->pos - 1;
	else
		pos = gate->pos + 1;

	if (pos < up_limit || pos + gate->size - 1 > down_limit)
		return -1;

	gate->pos = pos;
	drawn_gate(gate);
	if (up == true)
		mvaddch(pos + gate->size, x_door, BLANK);
	else
		mvaddch(pos - 1, x_door, BLANK);

	refresh();
	return 0;
}

void updata_ball_pos(struct ball *ball)
{
	ball->y_pos = ball->y_pos + 1 * ball->y_dir;
	ball->x_pos = ball->x_pos + 1 * ball->x_dir;
}

void bound(struct ball *ball)
{
	int x_next_pos;
	int y_next_pos;
	struct gate *gate = &the_window.gate;

	x_next_pos = ball->x_pos + 1 * ball->x_dir;
	y_next_pos = ball->y_pos + 1 * ball->y_dir;

	if (x_next_pos >= the_window.down_right_x) {
		if (y_next_pos >= gate->pos &&
		    y_next_pos <= gate->pos + gate->size - 1) {
			ball->x_dir = -1 * ball->x_dir;
			return;
		} else {
			clear();
			refresh();
			endwin();

			printf("  >> GAME OVER! <<\n");
			exit(0);	
		}
	}

	if (x_next_pos <= the_window.up_left_x)
		ball->x_dir = -1 * ball->x_dir;

	if (y_next_pos <= the_window.up_left_y ||
	    y_next_pos >= the_window.down_right_y)
		ball->y_dir = -1 * ball->y_dir;
}

void move_msg(int signum)
{
	int y_cur, x_cur, moved;

	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;

	bound(&the_ball);
	updata_ball_pos(&the_ball);

	mvaddch(y_cur, x_cur, BLANK);
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);

	refresh();
}

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void set_up(void)
{
	initscr();
	noecho();
	crmode();

	drawn_window(&the_window);
	refresh();

	signal(SIGALRM, move_msg);
	set_ticker(1000/TICKS_PER_SEC);
}

int main(void)
{
	char c;

	set_up();

	while (1) {
		c = getch();

		if (c == 'j')
			move_gate(&the_window.gate, 0);
		if (c == 'k')
			move_gate(&the_window.gate, 1);
		if (c == 'q')
			break;
	}
	endwin();

	return 0;
}
