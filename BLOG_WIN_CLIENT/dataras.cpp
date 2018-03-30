#include "dataras.h"

DataRAS::DataRAS(Q_TCP_Util *tcpsoc)
{
this->m_tcp=tcpsoc;
}
 BOOL DataRAS:: getin()
 {
    M_PACK* m_pac=new M_PACK;
    m_pac->pack_type=PACK_LOG;


    strcpy_s(m_pac->pack_body,m_log_in_id.length(),(char *)m_log_in_id.data());
    strcat_s(m_pac->pack_tail,m_log_in_id.length(),(char *)m_log_in_passward.data());
    m_tcp->f_send(m_pac);
    delete m_pac;
            /* QString m_log_in_id;
    QString m_log_in_passward;*/

     qDebug() << "util reciv!";
    return TRUE;
 }
 DataRAS::~DataRAS()
 {

 }
 int DataRAS::SendToSocket(M_PACK*str)
 {
    if(1== this->m_tcp->f_recv(str))
    {
        return 1;
    }
    else
     return 0;
 }
 int DataRAS::RecvFromSocket()
 {

     return 0;
 }
