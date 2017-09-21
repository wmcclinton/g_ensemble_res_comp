/*
 * Copyright 2016 Ahnaf Siddiqui
 *
 * This program uses the GROMACS molecular simulation package API.
 * Copyright (c) 1991-2000, University of Groningen, The Netherlands.
 * Copyright (c) 2001-2004, The GROMACS development team.
 * Copyright (c) 2013,2014, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed at http://www.gromacs.org.
 */

#include "gkut_log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static FILE *out_log = NULL;
static const char *error_prefix = "Fatal error in source file %s line %d:\n";

void gk_init_log(const char *logfile, int argc, char *argv[]) {
	out_log = fopen(logfile, "a");
	
	time_t t = time(NULL);
	struct tm *ltime = localtime(&t);

	fprintf(out_log, "\n");
	for(int i = 0; i < argc; ++i) {
		fprintf(out_log, "%s ", argv[i]);
	}
	fprintf(out_log, "\nRun: %d-%d-%d %d:%d:%d\n",
		ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_year + 1900,
		ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
}

void gk_close_log() {
	fclose(out_log);
}

void gk_print_log(char const *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
	if(out_log != NULL) {
		va_start(arg, fmt);
		vfprintf(out_log, fmt, arg);
		va_end(arg);
	}
}

void gk_log_fatal(int fatal_errno, const char *file, int line, char const *fmt, ...) {
	va_list arg;

	va_start(arg, fmt);
	printf(error_prefix, file, line);
	vprintf(fmt, arg);
	va_end(arg);

	if(out_log != NULL) {
		va_start(arg, fmt);
		fprintf(out_log, error_prefix, file, line);
		vfprintf(out_log, fmt, arg);
		va_end(arg);
	}
	// not using Gromacs's gmx_fatal function because it wasn't displaying data properly
	// va_start(arg, fmt);
	// gmx_fatal(fatal_errno, file, line, fmt, arg);
	// va_end(arg);
	exit(EXIT_FAILURE);
}
