# Makefile for the lab1 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	game
SRCS=	game.c testgame.c video_gr.c vbe.c keyboard.c timer.c read_xpm.c sprite.c vector.c mouse.c bmp.c map.c player.c list.c 
SRCS+=  bullet.c debug.c font.c objmanager.c texturemanager.c rectangle.c enemymanager.c enemy.c rtc.c menu.c uart.c networkmanager.c ball.c particle.c
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
