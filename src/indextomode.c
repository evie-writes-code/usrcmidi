/* index to mode
 * takes indices (scale degrees) starting from 1 and a mode (ionian, mixolydian, etc) and outputs the equivalent midi tone
 */

#include "m_pd.h"
#include <stdlib.h>

static t_class * indextomode_class;
//the basic pattern of whole steps and half steps that defines the modal space
static int modes[7] = {2, 2, 1, 2, 2, 2, 1};

typedef struct indextomode {
	t_object x_obj;
	t_inlet *modesetinlet;
	t_float currentmode;
} t_indextomode;

//handler for bang-type messages to this object
void indextomode_bang(t_indextomode *x) {
	post("indextomode set to mode %d of 7", (int) x->currentmode);
}

//handler for float-type input to the leftmost inlet
void indextomode_notein(t_indextomode *x, t_floatarg note) {
	t_float noteout = 0.0;
	for(int i = 0; i < note-1; i++) {
		noteout = noteout + modes[(i + ((int) x->currentmode - 1)) % 7];
	}
	outlet_float(x->x_obj.ob_outlet, noteout);
}

//freeing anything in the dataspace not already accounted for by
//the automatic garbage collection
void indextomode_free(t_indextomode *x) {
	inlet_free(x->modesetinlet);
}

//called when a new instance of our pd object is created
void *indextomode_new(void) {
	t_indextomode *x = (t_indextomode *) pd_new(indextomode_class);
	x->currentmode = 1.0;	//mode is initialised to ionian
	//creates a second inlet aside from the first which exists by default
	//this inlet only takes float messages, and writes the content 
	//directly to the local member 'currentmode'
	x->modesetinlet = floatinlet_new(&x->x_obj, &(x->currentmode));
	//creating an outlet which expects to be outputting float type messages
	outlet_new(&x->x_obj, &s_float);
	return (void *) x;
}

//called on pd startup when building the library of objects
void indextomode_setup(void) {
	indextomode_class = class_new(gensym("indextomode"),
		(t_newmethod) indextomode_new, (t_method) indextomode_free,
		sizeof(t_indextomode), CLASS_DEFAULT, 0);
	class_addfloat(indextomode_class, indextomode_notein);
	class_addbang(indextomode_class, indextomode_bang);
}