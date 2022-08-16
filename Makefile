#######################################################################
#
# Copyright (C) 2020-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

CFLAGS = -g -Wall 
CC = gcc

# Assignment No.
a = 4

default: scheduler

scheduler: *.c simulator.a queue.a
	$(CC) -o $@ $^ $(CFLAGS) 2>&1 | tee make.out

grade: clean scheduler 
	@./grade.sh $a

clean:
	@rm -f scheduler *.out

submit: clean
	@echo ""
	@echo "#### Only these file are submitted:"
	@echo ""
	@tar czvf ~/CSE130-Assignment$(a).tar.gz \
		--exclude=scheduler.h --exclude=simulator.h --exclude=queue.h *.h *.c
	@echo ""
	@echo "##### Confirming submission is good..."
	@echo ""
	@./confirm.sh $(a)
	@echo "#### If that failed, fix the problem and try again."
	@echo ""
	@echo "Finally, don't forget to upload ~/CSE130-Assignment$(a).tar.gz to Canvas!"
	@echo ""
