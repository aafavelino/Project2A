cd cdt
make all
make lib
make clean
cd ..

cd src 
make
make clean
cd ..


#
# Se você tem Doxygen instalado no seu computador, habilite os comandos abaixo:
#
#cd doc
#doxygen config.dox
#cd ..
