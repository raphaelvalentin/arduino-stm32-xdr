/* @(#)xdr_mem.c	2.1 88/07/29 4.0 RPCSRC */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr_mem.c 1.19 87/08/11 Copyr 1984 Sun Micro";
#endif

/*
 * xdr_mem.h, XDR implementation using memory buffers.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * If you have some data to be interpreted as external data representation
 * or to be converted to external data representation in a memory buffer,
 * then this is the package for you.
 *
 */

#include "src/config.h"

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include <string.h>

#include "src/types.h"
#include "src/xdr.h"


#include "src/byteswap.h"

static bool_t	xdrmem_getlong(XDR *, long *);
static bool_t	xdrmem_putlong(XDR *, const long *);
static bool_t	xdrmem_getbytes(XDR *, char *, u_int);
static bool_t	xdrmem_putbytes(XDR *, const char *, u_int);
static u_int	xdrmem_getpos(XDR *);
static bool_t	xdrmem_setpos(XDR *, u_int);
static long *	xdrmem_inline(XDR *, u_int);
static void	    xdrmem_destroy(XDR *);

struct xdr_ops {
    bool_t (*xdrmem_getlong)(XDR *, long *);
    bool_t (*xdrmem_putlong)(XDR *, const long *);
    bool_t (*xdrmem_getbytes)(XDR *, char *, u_int);
    bool_t (*xdrmem_putbytes)(XDR *, const char *, u_int);
    u_int (*xdrmem_getpos)(XDR *);
    bool_t (*xdrmem_setpos)(XDR *, u_int);
    long * (*xdrmem_inline)(XDR *, u_int);
    void (*xdrmem_destroy)(XDR *);
};

static const struct xdr_ops xdrmem_ops =
{
    xdrmem_getlong,
    xdrmem_putlong,
    xdrmem_getbytes,
    xdrmem_putbytes,
    xdrmem_getpos,
    xdrmem_setpos,
    xdrmem_inline,
    xdrmem_destroy
};


/*
 * The procedure xdrmem_create initializes a stream descriptor for a
 * memory buffer.
 */
void
xdrmem_create(XDR * xdrs, caddr_t addr, u_int size, enum xdr_op op)
{
	xdrs->x_op = op;
	xdrs->x_ops = (XDR::xdr_ops *) &xdrmem_ops;
	xdrs->x_private = xdrs->x_base = addr;
	xdrs->x_handy = size;
}

void
xdrmem_destroy(XDR * xdr)
{
    UNUSED(xdr);
}

bool_t
xdrmem_getlong(XDR * xdrs, long * lp)
{

	if ((xdrs->x_handy -= sizeof(long)) < 0)
		return (FALSE);
	*lp = (long)ntohl((u_long)(*((long *)(xdrs->x_private))));
	xdrs->x_private += sizeof(long);
	return (TRUE);
}

bool_t
xdrmem_putlong(XDR * xdrs, const long * lp)
{

	if ((xdrs->x_handy -= sizeof(long)) < 0)
		return (FALSE);
	*(long *)xdrs->x_private = (long)htonl((u_long)(*lp));
	xdrs->x_private += sizeof(long);
	return (TRUE);
}

bool_t
xdrmem_getbytes(XDR * xdrs, char * addr, u_int len)
{

	if ((xdrs->x_handy -= len) < 0)
		return (FALSE);
	memcpy (addr, xdrs->x_private, len);
	xdrs->x_private += len;
	return (TRUE);
}

bool_t
xdrmem_putbytes(XDR * xdrs, const char * addr, u_int len)
{

	if ((xdrs->x_handy -= len) < 0)
		return (FALSE);
	memcpy (xdrs->x_private, addr, len);
	xdrs->x_private += len;
	return (TRUE);
}

u_int
xdrmem_getpos(XDR * xdrs)
{

	return xdrs->x_private - xdrs->x_base;
}

bool_t
xdrmem_setpos(XDR * xdrs, u_int pos)
{
	register caddr_t newaddr = xdrs->x_base + pos;
	register caddr_t lastaddr = xdrs->x_private + xdrs->x_handy;

	if ((long)newaddr > (long)lastaddr)
		return (FALSE);
	xdrs->x_private = newaddr;
	xdrs->x_handy = lastaddr - newaddr;
	return (TRUE);
}

long *
xdrmem_inline(XDR * xdrs, u_int len)
{
	long *buf = 0;

	if (xdrs->x_handy >= len) {
		xdrs->x_handy -= len;
		buf = (long *) xdrs->x_private;
		xdrs->x_private += len;
	}
	return (buf);
}

#ifdef __cplusplus
}
#endif
