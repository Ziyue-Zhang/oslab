#define NALLOC 1024
typedef long Align;
typedef union header {
	struct {
		union header *next;
		unsigned size;
	}s;
	Align x;
}Header;

static Header base;
static Header *freep = NULL;
void ffree(void *ap);
void *mmalloc(unsigned nbytes);
static Header *morecore(unsigned nu);

char *sbrk(int size){
	if(_heap.start + size > _heap.end)
		return (char*)-1;
	else
		return (char*) _heap.start;
}
void *mmalloc(unsigned nbytes) {
	Header *p, *prevp;
	unsigned nunits = (nbytes+sizeof(Header)-1)/sizeof(Header)+1;
    if((prevp=freep)==NULL) {
		base.s.next = freep->s.next = prevp->s.next = &base;
		base.s.size = 0;
	}
	for (p = prevp->s.next;;prevp = p, p=p->s.next) {
		if(p->s.size>=nunits) {
			if(p->s.size==nunits)
				prevp->s.next=p->s.next;
			else {
				p->s.size-=nunits;
				p+=p->s.size;
				p->s.size=nunits;
			}
			freep=prevp;
			return (void*) (p+1);
		}
		if(p==freep)
			if((p=morecore(nunits))==NULL)
				return NULL;
	}
}
static Header *morecore(unsigned nu) {
	char *cp, *sbrk(int);
	Header *up;
	if(nu < NALLOC)
		nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));
	if(cp == (char *) -1)
		return NULL;
	up = (Header *) cp;
	up->s.size=nu;
	ffree((void *)(up+1));
	return freep;
}
void ffree(void *ap)
{
	Header *bp, *p;
	bp = (Header *)ap - 1;
	for(p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
		if(p >= p->s.next && (bp > p || bp < p->s.next))
			break;
	if(bp +bp->s.size == p->s.next) {
		bp->s.size += p->s.next->s.size;
		bp->s.next = p->s.next->s.next;
	}
	else
		bp->s.next = p->s.next;
	if(p + p->s.size == bp) {
		p->s.size += bp->s.size;
		p->s.next = bp->s.next;
	}
	else
		p->s.next = bp;
	freep = p;
}
