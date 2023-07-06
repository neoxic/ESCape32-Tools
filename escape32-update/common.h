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

#pragma once

#include <stdint.h>

int openserial(const char *path);
int recv(int fd, uint8_t *buf, int len);
void send(int fd, const uint8_t *buf, int len);
int recvval(int fd);
void sendval(int fd, int val);
int recvdata(int fd, uint8_t *buf);
void senddata(int fd, const uint8_t *buf, int len);
uint32_t crc32(const uint8_t *buf, int len);
