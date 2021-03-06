#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdd_parser.h"
#include "sdd_builder.h"


void sdd_state_init(sdd_state* s, const char* name, const char* entries, const char* exits, const char* default_stub) {
    s->name         = strdup(name);
    s->entries      = strdup(entries);
    s->exits        = strdup(exits);
    s->default_stub = strdup(default_stub);
}

void sdd_state_release(sdd_state* s) {
    free((void*)s->name);
    free((void*)s->entries);
    free((void*)s->exits);
    free((void*)s->default_stub);
}

sdd_transition* sdd_transition_new(const char* from, const char* to, const char* event, const char* conditions, const char* actions) {
	sdd_transition* t = malloc(sizeof(sdd_transition));
	t->from       = strdup(from);
	t->to         = strdup(to);
	t->event      = strdup(event);
	t->conditions = strdup(conditions);
	t->actions    = strdup(actions);
	return t;
}

void sdd_transition_delete(sdd_transition* t) {
	free((void*)t->actions);
	free((void*)t->conditions);
	free((void*)t->event);
	free((void*)t->to);
	free((void*)t->from);
	free(t);
}

void sdd_transition_delete(sdd_transition* transition);


typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern void yy_switch_to_buffer(struct yy_buffer_state *);
extern SDDBuilder __secret_builder;

void markdown(const char* tag_name, const char* content) {
	static const char* tags[] = {
		// "entry", 
		// "exit",
		// "default",
		// "ids", 
		// "id_groups", 
		// "procedure",
		// "state_actions", 
		// "state_name",
		// "stub",
		// "cluster",
		// "bucket",
		// "state",
		// "condition",
		// "trans",
		// "post_acts",
	};

	int acceptable = 0;
	for (int i=0; i<sizeof(tags)/sizeof(tags[0]); ++i) {
	    acceptable = (acceptable || strcmp(tags[i], tag_name) == 0) ? 1 : 0;
	}

	if (acceptable != 0)
	    printf("<%-12s> %s\n", tag_name, content);
} 


void sdd_parse(const char *dsl, sdd_parser_callback* callback) {
	SDDBuilderInit(&__secret_builder, &markdown, callback);

	YY_BUFFER_STATE dsl_buffer = yy_scan_string((char*)dsl);
	yy_switch_to_buffer( dsl_buffer); // switch flex to the buffer we just created 
	yyparse(); 
	yy_delete_buffer(dsl_buffer);

	// SDDDumpParser(&__secret_builder, "Status");
	SDDBuilderDestroy(&__secret_builder);
}