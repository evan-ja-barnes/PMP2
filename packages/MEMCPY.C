memcpy(dest, src, len)
register char *dest, *src;
register int len;
{
if(src < dest)
	{
	src += len;
	dest += len;
	while(len-- > 0)
		*--dest = *--src;
	}
else	while(len-- > 0)
		*dest++ = *src++;
}
