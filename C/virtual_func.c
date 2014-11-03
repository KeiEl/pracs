#include <stdlib.h>
#include <stdio.h>

// DO NOT EVER HAVE CAR INHERITED FROM ENGINE
/* If b is not allocated on the heap, 
	when derived1 is allocated on the stack, b -> stack/same lifetime?
	when derived1 is allocated on the heap, b -> stack/different lifetime?
*/

/*
	TAKE 1:
*/

// struct engine {
// 	struct vtable *table;	
// };

// struct car {
// 	struct engine e;
// 	int data;
// };

// struct vtable {
// 	void (*make_sound)( struct engine *e, int);
// } engine_vt, car_vt;

// void do_nothing( struct engine *e, int num ) {
// 	printf( "Vroom vroom~\n" );
// }

// void do_something( struct engine *e, int num ) {
// 	printf( "data + num = %d\n", ((struct car*)e)->data + num );
// }

// void act( struct engine *e ) {
// 	e->table->make_sound(e,2);
// }

/*
	TAKE 2: Add function ptr "this"?
*/

struct engine {
	struct engine *this;
	struct vtable *table;	
};

struct car {
	struct car *this;
	struct engine e;
	int data;
};

struct vtable {
	void (*make_sound)( struct engine *e, int);
} engine_vt, car_vt;

void do_nothing( struct engine *e, int num ) {
	printf( "Vroom vroom~\n" );
}

void do_something( struct engine *e, int num ) {
	printf( "data + num = %d\n", ((struct car*)e)->data + num );
}

void act( struct engine *e ) {
	e->table->make_sound(e,2);
}





struct engine* new_engine() {
	struct engine *e = (struct engine*)malloc( sizeof(struct engine) );
	e->table = &engine_vt;
	return e;
}

struct car* new_car( int data ) {
	struct car *c = (struct car*)malloc( sizeof(struct car) );
	c->e.table = &car_vt;
	c->data = data;
	return c;
}

int main() {
	engine_vt.make_sound = do_nothing;
	car_vt.make_sound = do_something;

	struct engine *e = (struct engine*)new_engine();
	struct engine *c = (struct engine*)new_car(2);

	act(e);
	act(c);

	return 0;
}