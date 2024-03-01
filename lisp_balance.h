#ifndef LISP_BANALNCE_H
#define LISP_BANALNCE_H
// Return values:
enum {
    FULL = 0, // the provided statement seems complete
    PARTIAL,  // the provided statement is not closed
    BROKEN,   // the provided is all fucked'n'shieet
};
#endif
