

#ifndef ZONE_UTILS_H
#define ZONE_UTILS_H

namespace gpp
{

struct ZoneSpacer;
struct ZoneColision;

struct ZoneSpacer
{
gpp_zone* spatialize(std::vector<gpp_zone*>& zones)
{
struct ZoneSorter
{
bool operator()(gpp_zone* z1, gpp_zone* z2)
{
return !(z1->getBody()->getArea()<z2->getBody()->getArea());
}
};
if(zones.size()==0)
{
return NULL;
}
if(zones.size()==1)
{
return zones[0];
}
std::sort(zones.begin(), zones.end(), ZoneSorter());
gpp_zone* root=zones[0];
for(auto& it : zones)
{
if(it->index==root->index)
{
continue;
}
internalInsert(root, it);
}
return root;
}
bool internalInsert(gpp_zone* root, gpp_zone* sf)
{
if((root==NULL)||(sf==NULL))
{
return false;
}
BoundingBox2d broot=root->getBody()->getBoundingBox();
BoundingBox2d child=sf->getBody()->getBoundingBox();
if((broot.canContains(&child))&&(broot.isContained(&child))&&(root->childs.size()==0))
{
root->addChild(sf);
return true;
}
if(root->childs.size()==0)
{
return false;
}
BoundingBox2d b1=child;
for(auto& it : root->childs)
{
BoundingBox2d b2=it->getBody()->getBoundingBox();
if((b2.canContains(&b1))&&(b2.isContained(&b1)))
{
if(this->internalInsert(it, sf))
{
return true;
}
else
{
return false;
}
}
if((b1.m1.x>b2.m2.x)||(b2.m1.y>b2.m4.y))
{
continue;
}
if((b1.m1.x<b2.m1.x)&&(b1.m2.x>b2.m1.x))
{
b1.m2.x=b2.m1.x-1.0f;
}
if((b1.m1.x<b2.m2.x)&&(b1.m2.x>b2.m2.x))
{
b1.m1.x=b2.m2.x+1;
}
if((b1.m1.y<b2.m1.y)&&(b1.m2.y>b2.m1.y))
{
b1.m2.y=b2.m1.y-1.0f;
}
if((b1.m1.y<b2.m2.y)&&(b1.m2.y>b2.m2.y))
{
b1.m1.y=b2.m2.y+1;
}
}
b1.recalculate();
if(b1.canContains(&child))
{
root->addChild(sf);
return true;
}
return false;
}
};

struct ZoneColision
{
bool operator()(const std::vector<gpp_zone*>& zones, gpp_zone* sf, gpp_zone** colided)
{
if((zones.size()==0)||(sf==NULL))
{
return false;
}
for(auto& it : zones)
{
if(it->index==sf->index)
{
continue;
}
if(it->getBody()->isColiding(sf->getBody()))
{
if(colided!=NULL)
{
*colided=it;
}
return true;
}
}
return false;
}
};
}
#endif
