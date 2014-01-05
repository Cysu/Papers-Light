#ifndef CONST_H
#define CONST_H

const int BUFSIZE = 1024;

#ifdef Q_OS_WIN
    const char* const PREFERENCE_FILENAME = ".plrc.txt";
#else
    const char* const PREFERENCE_FILENAME = ".plrc";
#endif

#endif // CONST_H
