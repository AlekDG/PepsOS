#docker start CATEDRA
#docker exec -it CATEDRA make clean -C/root/Toolchain
#docker exec -it CATEDRA make clean -C/root/
#docker exec -it CATEDRA make -C/root/Toolchain
#docker exec -it CATEDRA make -C/root/
#docker stop CATEDRA

#una vez dentro del proyecto, en la terminal de docker:
#cd /root
#cd Toolchain
#make all
#cd ..
#make all

cd ./Toolchain/
make clean all
cd ..
make clean all
./run.sh