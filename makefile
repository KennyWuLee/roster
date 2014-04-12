CC = gcc
EXEC = roster

${EXEC}: roster.c Student.h
	${CC} -Wall roster.c -o ${EXEC}

clean:
	rm -f ${EXEC}
