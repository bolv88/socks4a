#define SOCKS4REQLEN 262
int main(int argc, char *argv[])
{
    unsigned char socksreq[SOCKS4REQLEN];

    char hostname[] = "www.okbuy.com";
    int remote_port = 5005;
    bool protocol4a = true;

    unsigned short ip[4]={106,187,49,213};

    /*--------------------------------------------*/

    socksreq[0] = 4; /* version (SOCKS4) */
    socksreq[1] = 1; /* connect */
    socksreq[2] = (unsigned char)((remote_port >> 8) & 0xff); /* PORT MSB */
    socksreq[3] = (unsigned char)(remote_port & 0xff); /* PORT LSB */
    /* set DSTIP*/
    socksreq[4] = (unsigned char)ip[0];
    socksreq[5] = (unsigned char)ip[1];
    socksreq[6] = (unsigned char)ip[2];
    socksreq[7] = (unsigned char)ip[3];

    socksreq[8] = 0;/* ensure empty userid is NUL-terminated */


    int packetsize = 9 +
              (int)strlen((char*)socksreq + 8); /* size including NUL */

    if (protocol4a) {
        /* If SOCKS4a, set special invalid IP address 0.0.0.x */
        socksreq[4] = (unsigned char)ip[0];
        socksreq[5] = (unsigned char)ip[1];
        socksreq[6] = (unsigned char)ip[2];
        socksreq[7] = (unsigned char)ip[3];
        /* If still enough room in buffer, also append hostname */
        ssize_t hostnamelen = 0;
        int packetsize = 9+(int)strlen((char*)socksreq+8);
        hostnamelen = (ssize_t) strlen(hostname)+1;
        if (packetsize+hostnamelen < SOCKS4REQLEN) {
            strcpy((char*)socksreq+packetsize, hostname);
        } else {
            hostnamelen = 0; /* Flag: hostname did not fit in buffer */
        }
    }

    return 0;
}
