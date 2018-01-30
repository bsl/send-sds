#include "midi.h"
#include <stddef.h>
#include <assert.h>

void
open_close(void);

void
open_close(void)
{
    midi_t midi = NULL;
    err_t err = err_create(100);
    int res = midi_open_interface("hw:0", &midi, err);
    if (res) {
        assert( midi != NULL );
    }
    else {
        assert( err_get(err) != NULL );
    }
    err_destroy(err);
}

int
main(void)
{
  open_close();
}
