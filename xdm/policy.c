/*

Copyright 1988, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/

/*
 * xdm - display manager daemon
 * Author:  Keith Packard, MIT X Consortium
 *
 * policy.c.  Implement site-dependent policy for XDMCP connections
 */

#include "dm.h"
#include "dm_auth.h"

#include <errno.h>

#ifdef XDMCP

# include <X11/X.h>

# include "dm_socket.h"

static ARRAY8 noAuthentication = { (CARD16) 0, (CARD8Ptr) 0 };

typedef struct _XdmAuth {
    ARRAY8  authentication;
    ARRAY8  authorization;
} XdmAuthRec, *XdmAuthPtr;

static XdmAuthRec auth[] = {
# ifdef HASXDMAUTH
{ {(CARD16) 20, (CARD8 *) "XDM-AUTHENTICATION-1"},
  {(CARD16) 19, (CARD8 *) "XDM-AUTHORIZATION-1"},
},
# endif
{ {(CARD16) 0, (CARD8 *) 0},
  {(CARD16) 0, (CARD8 *) 0},
}
};

# define NumAuth	(sizeof auth / sizeof auth[0])

ARRAY8Ptr
ChooseAuthentication (ARRAYofARRAY8Ptr authenticationNames)
{
    int	i, j;

    for (i = 0; i < (int)authenticationNames->length; i++)
	for (j = 0; j < NumAuth; j++)
	    if (XdmcpARRAY8Equal (&authenticationNames->data[i],
				  &auth[j].authentication))
		return &authenticationNames->data[i];
    return &noAuthentication;
}

int
CheckAuthentication (
    struct protoDisplay	*pdpy,
    ARRAY8Ptr		displayID,
    ARRAY8Ptr		name,
    ARRAY8Ptr		data)
{
# ifdef HASXDMAUTH
    if (name->length && !strncmp ((char *)name->data, "XDM-AUTHENTICATION-1", 20))
	return XdmCheckAuthentication (pdpy, displayID, name, data);
# endif
    return TRUE;
}

int
SelectAuthorizationTypeIndex (
    ARRAY8Ptr		authenticationName,
    ARRAYofARRAY8Ptr	authorizationNames)
{
    int	i, j;

    for (j = 0; j < NumAuth; j++)
	if (XdmcpARRAY8Equal (authenticationName,
			      &auth[j].authentication))
	    break;
    if (j < NumAuth)
    {
	for (i = 0; i < (int)authorizationNames->length; i++)
	    if (XdmcpARRAY8Equal (&authorizationNames->data[i],
				  &auth[j].authorization))
		return i;
    }
    for (i = 0; i < (int)authorizationNames->length; i++)
	if (ValidAuthorization (authorizationNames->data[i].length,
				(char *) authorizationNames->data[i].data))
	    return i;
    return -1;
}

/*ARGSUSED*/
int
Willing (
    ARRAY8Ptr	    addr,
    CARD16	    connectionType,
    ARRAY8Ptr	    authenticationName,
    ARRAY8Ptr	    status,
    xdmOpCode	    type)
{
    char	statusBuf[256];
    int		ret;

    ret = AcceptableDisplayAddress (addr, connectionType, type);
    if (!ret)
	snprintf (statusBuf, sizeof(statusBuf),
		  "Display not authorized to connect");
    else
    {
        if (*willing)
	{   FILE *fd = NULL;
	    if ((fd = popen(willing, "r")))
	    {
		char *s = NULL;
		errno = 0;
		while(!(s = fgets(statusBuf, 256, fd)) && errno == EINTR)
			errno = 0;
		if (s && strlen(statusBuf) > 0)
			statusBuf[strlen(statusBuf)-1] = 0; /* chop newline */
		else
			snprintf (statusBuf, sizeof(statusBuf),
				  "Willing, but %s failed", willing);
	    }
	    else
	        snprintf (statusBuf, sizeof(statusBuf),
			  "Willing, but %s failed", willing);
	    if (fd) pclose(fd);
	}
	else
	    snprintf (statusBuf, sizeof(statusBuf), "Willing to manage");
    }
    status->length = strlen (statusBuf);
    status->data = calloc (status->length, sizeof (*status->data));
    if (!status->data)
	status->length = 0;
    else
	memmove( status->data, statusBuf, status->length);
    return ret;
}

static ARRAY8 noAccessACL = { (CARD16) 33, (CARD8Ptr) "Display not authorized to connect" };

/*ARGSUSED*/
ARRAY8Ptr
Accept (
    ARRAY8Ptr	    addr,
    CARD16	    connectionType,
    CARD16	    displayNumber)
{
    int		    ret;

    /*
     * Probably the biggest gapping security hole in XDM: all one has to do is
     * skip the Query/Willing exchange and move directly to Request/Accept to
     * completely bypass the ACL.
     */
    ret = AcceptableDisplayAddress (addr, connectionType, REQUEST);
    if (!ret)
	return &noAccessACL;

    return NULL;
}

/*ARGSUSED*/
int
SelectConnectionTypeIndex (
    ARRAY8Ptr	     addr,
    CARD16	     family,
    ARRAY16Ptr	     connectionTypes,
    ARRAYofARRAY8Ptr connectionAddresses,
    CARD16Ptr	     connectionType,
    ARRAY8Ptr	     connectionAddress)
{
    int i, ret = -1;

    /*
     * Select one supported connection type
     */

    /*
     * I suppose this was written when it was not usual to have more than one IP
     * address or netowrk interface for a host.  I have a machine with 2 NICs
     * with an IPv4LL allocated address on one NIC that is disconnected, and a
     * regular IPv4 address on the other.  Both NICs have IPv6 link scope
     * addresses assigned.  Xorg servers are sending 4 addresses in the list
     * when looping back on IPv4: with the non-function IPv4LL address first in
     * the list.  This function would never let it connect.
     */

    for (i = 0; i < connectionTypes->length; i++) {
	switch (connectionTypes->data[i]) {
	  case FamilyLocal:
# if defined(TCPCONN)
	  case FamilyInternet:
#  if defined(IPv6) && defined(AF_INET6)
	  case FamilyInternet6:
#  endif /* IPv6 */
# endif /* TCPCONN */
	    if (family == connectionTypes->data[i] &&
		XdmcpARRAY8Equal(addr, &connectionAddresses->data[i])) {
		/* if the address is the same as the request address, use it */
		*connectionType = connectionTypes->data[i];
		connectionAddress->length = connectionAddresses->data[i].length;
		connectionAddress->data = connectionAddresses->data[i].data;
		return i;
	    }
	    if (ret == -1) {
		/* tentatively use the first usable address */
		*connectionType = connectionTypes->data[i];
		connectionAddress->length = connectionAddresses->data[i].length;
		connectionAddress->data = connectionAddresses->data[i].data;
		ret = i;
	    }
	}
    } /* for */
    /*
     * This is really only meant to handle old xqproxy which placed zero
     * connections in the connection list, or for situations with Xorg servers
     * such as: X :%d -nolisten tcp -indirect localhost, in which case there
     * will be no connections in the connection list for Xorg servers.
     */
    if (ret == -1 && isLocalAddress(addr, family)) {
	/* maybe FamilyLocal or FamilyLocalHost here? */
	*connectionType = family;
	connectionAddress->length = addr->length;
	connectionAddress->data = addr->data;
	ret = connectionTypes->length;
    }
    return (ret);
}

#endif /* XDMCP */
