/* mono
 * Evie Campbell, April 17, 2018
 * object designed to imitate the functionality of monophonic synths with 'stack' style voice stealing
 */

#include "m_pd.h"
#include <stdlib.h>

static t_class *mono_class;

typedef struct node {
	struct node *next;
	float note;
	float velocity;
} node;

typedef struct mono {
	t_object x_obj;
	node *head;
} t_mono;

//formats a list-type puredata message to be output
t_atom *makeList(float note, float velocity) {
	t_atom *outputList = malloc(sizeof(t_atom) * 2);
	SETFLOAT(&outputList[0], note);
	SETFLOAT(&outputList[1], velocity);
	return outputList;
}

//pushes a new note/velocity pair onto the head of the stack
void pushNote(t_mono *x, float note, float velocity) {
	node *prevHead = x->head;
	node *newHead = malloc(sizeof(node));
	if(newHead == NULL) {
		post("mono: error requesting memory.");
	}
	else {
		if(prevHead == NULL) {
			newHead->next = NULL;
		}
		else {
			newHead->next = prevHead;
		}
		newHead->note = note;
		newHead->velocity = velocity;
		x->head = newHead;
	}
}

//searches the stack and removes any note/velocity pairs with a matching note, then sends the new state to the outlet
void popNote(t_mono *x, float note) {
	if(x->head == NULL) {
		return;
	}
	else {
		while(x->head != NULL && x->head->note == note) {
			if(x->head->next == NULL) {
				outlet_list(x->x_obj.ob_outlet, &s_list, 2, makeList(x->head->note, 0));
				free(x->head);
				x->head = NULL;
				return;
			}
			else {
				node *newHead = x->head->next;
				free(x->head);
				x->head = newHead;
			}
		}
		if(x->head->next != NULL) {
			node *previous = x->head;
			node *current = previous->next;
			do {
				if(current->note == note) {
					previous->next = current->next;
					free(current);
					current = previous->next;
				}
				else {
					previous = current;
					current = current->next;
				}
			} while(current != NULL);
		}
		outlet_list(x->x_obj.ob_outlet, &s_list, 2, makeList(x->head->note, x->head->velocity));
	}
}

//takes (note, velocity) pairs and outputs (note, velocity) pairs
void mono_midiIn(t_mono *x, t_symbol *s, int argc, t_atom *argv) {
	if(argc == 2 && s == &s_list) {
		float note = atom_getfloat(&argv[0]);
		float velocity = atom_getfloat(&argv[1]);
		if(velocity != 0) {
			pushNote(x, note, velocity);
			outlet_list(x->x_obj.ob_outlet, &s_list, 2, makeList(note, velocity));
		}
		else {
			popNote(x, note);
		}
	}
	else {
		post("mono: error, wrong number of inputs. Takes (note, velocity) pairs.");
	}
}

void mono_free(t_mono *x) {
	if(x->head == NULL) {
		return;
	}
	else {
		node *current = x->head;
		while(current != NULL) {
			node *next = current->next;
			free(current);
			current = next;
		}
	}
}

void *mono_new() {
	t_mono *x = (t_mono *) pd_new(mono_class);
	x->head = NULL;
	outlet_new(&x->x_obj, &s_list);
	return (void *)x;
}

void mono_setup(void) {
	mono_class = class_new(gensym("mono"),
		(t_newmethod)mono_new,
		(t_method)mono_free, sizeof(t_mono),
		CLASS_DEFAULT,
		A_DEFFLOAT, 0);

	class_addlist(mono_class, mono_midiIn);
}