IAR ?= "/opt/IAR2/common/bin/iarbuild.exe"

all:
	./rev.sh
	wine $(IAR) NATICQ.ewp -make Release -log warnings
	wine $(IAR) NATICQ.ewp -make Release_SGOLD -log warnings
	wine $(IAR) NATICQ.ewp -make Release_ELKA -log warnings
#	wine $(IAR) NATICQ.ewp -make Release_X75 -log warnings
