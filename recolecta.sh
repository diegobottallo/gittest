#!/bin/sh
#
# Este script recolecta información de estado del sistema.
# Su ejecución periódica forma muestras estadísticas.
#
## DATOS IMPORTANTES A ESTABLECER
#
# Directorio donde se almacenan los archivos log
LOG_PATH=/opt/ENS
#
# Cantidad de procesadores de sistema
NPROCS=64
#
# --------------------------------
VERSION=2012.1
VERSION_FILE=$LOG_PATH/version.file
MPSTAT_FILE=$LOG_PATH/mpstat.file
IOSTAT_FILE=$LOG_PATH/iostat.file
VMSTAT_FILE=$LOG_PATH/vmstat.file
NETSTAT_I_FILE=$LOG_PATH/netstat_i.file
NETSTAT_N_FILE=$LOG_PATH/netstat_n.file
NETSTAT_N_SSH_FILE=$LOG_PATH/netstat_n_ssh.file
PRSTAT_FILE=$LOG_PATH/prstat.file
PRSTAT_M_FILE=$LOG_PATH/prstat_m.file
USERS_FILE=$LOG_PATH/users.file
TOP_FILE=$LOG_PATH/top.file
DF_FILE=$LOG_PATH/df_h.file

while :
do 
HORA=`date`
echo $VERSION $HORA >> $VERSION_FILE
echo $HORA >> $TOP_FILE
/usr/src/desa/util/top -b -n 0 >> $TOP_FILE
 
# --------------------------------
# Procesadores
LINEAS=`echo $NPROCS + 1 | bc`
echo $HORA >> $MPSTAT_FILE
mpstat 1 2 | tail -$LINEAS >> $MPSTAT_FILE
 
# --------------------------------
# Discos
echo $HORA >> $IOSTAT_FILE
iostat -xn 1 2 >> $IOSTAT_FILE
echo $HORA >> $DF_FILE
df -h >> $DF_FILE
 
# --------------------------------
# Memoria
echo $HORA >> $VMSTAT_FILE
vmstat 1 2 | cat -n >> $VMSTAT_FILE
 
# --------------------------------
# Red
echo $HORA >> $NETSTAT_I_FILE
netstat -i >> $NETSTAT_I_FILE 
 
netstat -n -f inet > $LOG_PATH/netstat.tmp
echo $HORA >> $NETSTAT_N_FILE
for ST in BOUND CLOSED CLOSING CLOSE_WAIT ESTABLISHED FIN_WAIT_1 FIN_WAIT_2 IDLE LAST_ACK LISTEN SYN_RECEIVED SYN_SENT TIME_WAIT
do
        echo $ST `grep "$ST" $LOG_PATH/netstat.tmp | wc -l` >> $NETSTAT_N_FILE
done
 
echo $HORA >> $NETSTAT_N_SSH_FILE 
netstat -n -P tcp | egrep 22 >> $NETSTAT_N_SSH_FILE

# --------------------------------
# Procesos
echo $HORA >> $PRSTAT_FILE
prstat -a -n 500 1 1 | sed -e '/^$/d' >> $PRSTAT_FILE
 
echo $HORA >> $PRSTAT_M_FILE
prstat -m -n 40 1 1 | sed -e '/^$/d' >> $PRSTAT_M_FILE
 
# --------------------------------
# Usuarios
echo $HORA >> $USERS_FILE
w >> $USERS_FILE
 
# --------------------------------

sleep 300
done
