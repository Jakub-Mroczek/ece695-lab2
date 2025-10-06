default:
	cd makeprocs; make
	cd n3_inject; make
	cd h2o_inject; make
	cd reaction_1; make
	cd reaction_2; make
	cd reaction_3; make

clean:
	cd makeprocs; make clean
	cd n3_inject; make clean
	cd h2o_inject; make clean
	cd reaction_1; make clean
	cd reaction_2; make clean
	cd reaction_3; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 7 6; ee469_fixterminal
