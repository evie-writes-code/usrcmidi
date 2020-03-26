/* index to mode
 * takes indices (scale degrees) starting from 1 and a mode (ionian, mixolydian, etc) and outputs the equivalent midi tone
 */

#include "m_pd.h"
#include <stdlib.h>

static t_class * indexToMode_class;

typedef struct indexToMode {
	t_object x_obj;
} t_indexToMode;

void indexToMode_setup(void) {

}

void *indexToMode_new() {

}

void indexToMode_free(t_indexToMode * x) {

}