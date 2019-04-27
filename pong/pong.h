/*
 * v0.1 2013.12.8 Sherlock some settings about pong
 */
#define BLANK         ' '
#define DEL_SYMBOL    'o'
#define DEL_LINE      '@'
#define X_INIT        10
#define Y_INIT        10
/* times of interrupts per second */
#define TICKS_PER_SEC 10
#define GATE_SIZE     8

/* ping pong ball */
struct ball {
	int y_pos, x_pos,
	    y_dir, x_dir;
	char symbol;
};

struct gate {
	int pos;
	int size;
	struct window *window;
};

/*
 *    +--> x
 *    |   up_left
 *    v        +-------------
 *    y        |             | gate
 *             |   o ball    |
 *             |             |
 *      window |
 *             +-------------
 *                         down_right
 */
struct window {
	int up_left_x,
	    up_left_y,
	    down_right_x,
	    down_right_y;
	struct gate gate;
};

