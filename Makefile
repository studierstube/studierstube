##
## Makefile
##  
## Made by Bernhard Reitinger
## Login   <breitinglocalhost.localdomain>
##
## Started on  Tue Feb 28 08:05:46 2006 Bernhard Reitinger
## Last update Tue Feb 28 08:05:46 2006 Bernhard Reitinger
## 

all:
	@echo "Making Studierstube using SCons"
	@scons -Q

install:
	@echo "Installing Studierstube using SCons"
	@scons -Q install

clean:
	@echo "Cleaning Studierstube using SCons"
	@scons -Q -c
