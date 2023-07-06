/*
** Copyright (C) 2022-2023 Arseny Vakhrushev <arseny.vakhrushev@me.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <err.h>
#include <fcntl.h>
#include <termios.h>

#define VERSION "rev1"

static const char *path = "/dev/ttyUSB0";

static int parseargs(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "d:h?")) != -1) {
		switch (opt) {
			case 'd':
				path = optarg;
				break;
			case 'h':
			case '?':
			default:
				return 0;
		}
	}
	return argc == optind;
}

int main(int argc, char *argv[]) {
	if (!parseargs(argc, argv)) {
		fprintf(stderr,
			"Usage: %s [options]\n"
			"Options:\n"
			"  -d <path>  Override path to serial device\n",
			argv[0]);
		return 1;
	}
	printf("ESCape32 CLI Utility " VERSION "\n");
	int fd = open(path, O_RDWR);
	if (fd == -1) err(1, "%s", path);
	struct termios ts;
	if (tcgetattr(fd, &ts)) goto error;
	cfmakeraw(&ts);
	if (cfsetspeed(&ts, B38400)) goto error;
	ts.c_cc[VMIN] = 0;
	ts.c_cc[VTIME] = 3; // 300ms read timeout
	if (tcsetattr(fd, TCSANOW, &ts)) goto error;
	if (tcflush(fd, TCIOFLUSH)) goto error;
	FILE *f = fdopen(fd, "r+");
	if (!f) goto error;
	char *buf = 0;
	size_t size = 0;
	printf("Press CTRL+D to quit\n");
	for (;;) {
		printf("> ");
		fflush(stdout);
		ssize_t len = getline(&buf, &size, stdin);
		if (len == -1) { // EOF
			printf("\rQuit!\n");
			break;
		}
		if (!strcmp(buf, "\n")) continue; // Empty command
		if (!fwrite(buf, len, 1, f)) goto error;
		usleep(len * 260); // tcdrain() is unreliable
		if (tcdrain(fd)) goto error;
		clearerr(f); // Clear EOF
		while ((len = getline(&buf, &size, f)) != -1) {
			fwrite(buf, len, 1, stdout);
			if (!strcmp(buf, "OK\n") || !strcmp(buf, "ERROR\n")) break;
		}
	}
	free(buf);
	fclose(f);
	return 0;
error:
	err(1, "%s", path);
}
