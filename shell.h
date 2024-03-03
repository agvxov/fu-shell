#ifndef FU_SHELL_H
#define FU_SHELL_H

extern "C" {
    int fu_shell_meta(const char * const l);
    bool fu_judge(const char * const input);
    bool fu_interpret();
}

#endif
