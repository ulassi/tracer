#include ".\tPolyShape.h"

tPolyShape::tPolyShape(void)
{
}

tPolyShape::~tPolyShape(void) {
	if (this->shader)
		delete this->shader;
	this->shader = 0;
}
tPolyShape::tPolyShape(const tPolyShape &o) {
	*this = o;
}
tPolyShape & tPolyShape::operator= (const tPolyShape &right){
	this->mPolygonArray = right.mPolygonArray;
	this->mPolygonNormalArray = right.mPolygonNormalArray;
	this->mPolygonSurfaceIndex = right.mPolygonSurfaceIndex;
	this->mVertexArray = right.mVertexArray;
	this->mVertexNormalArray = right.mVertexNormalArray;
	this->boundingSphere = right.boundingSphere;
	this->mPolygonTree = right.mPolygonTree;
	this->mBVH = right.mBVH;
	this->transform = right.transform;
	this->setShader(right.shader);
	this->setUVMapper(right.uvmapper);
	this->transform = right.transform;
	return *this;
}
/*
	Retrieves the geometric normal at a point.
*/
vec3 tPolyShape::normal(vec3 &point) {
	return vec3();	
}
void tPolyShape::init() {
	//transform vertices
	size_t i=0; 
	matrix4 &TF = transform;
	while (i< mVertexArray.size()) {
		double4 &vert = mVertexArray.at(i);
		double4 v = vert;
	
		//TF
		vert.x = TF.m[0][0]*v.x + TF.m[0][1]*v.y + TF.m[0][2]*v.z + TF.m[0][3]*v.w;
		vert.y = TF.m[1][0]*v.x + TF.m[1][1]*v.y + TF.m[1][2]*v.z + TF.m[1][3]*v.w;
		vert.z = TF.m[2][0]*v.x + TF.m[2][1]*v.y + TF.m[2][2]*v.z + TF.m[2][3]*v.w;
		vert.w = TF.m[3][0]*v.x + TF.m[3][1]*v.y + TF.m[3][2]*v.z + TF.m[3][3]*v.w;
		i++;
	}
	this->boundingSphere.center += vec3(TF.m[0][3],TF.m[1][3],TF.m[2][3]);
	buildBVH();
}
/*
	Build BVH

	Makes deep copies of the polygons in mPolygonArray.
	Thus it can now be released, as can all other vertex data.

	Inserts all into the mBVH object in this class.
*/
void tPolyShape::buildBVH() {
    //use tShapeAABB to create an octtree
	
	//create root node
	vec3 min,max;
	min = this->boundingSphere.center - vec3(boundingSphere.r, boundingSphere.r, boundingSphere.r);
	max = this->boundingSphere.center + vec3(boundingSphere.r, boundingSphere.r, boundingSphere.r);
	tAABB *box = new tAABB(min,max);
	mBVH.BV = box;
	std::vector<tPolygon*> primVec;
	for (size_t i=0; i<mPolygonArray.size(); i++) {
		//make sure vertex pointers in tPolygon is correct
		for (size_t j=0; j<mPolygonArray.at(i).vertices.size(); j++)
			mPolygonArray.at(i).vertices.at(j) = 
				&mVertexArray.at(mPolygonArray.at(i).vertexIndicies.at(j));

		//push prims to mBVH.prims
		primVec.push_back( &mPolygonArray[i]);	//pointer only!
	}
	numPolysBVH = 0;
	mBVH.depth = 0;
	addPolysToBVH(mBVH, *box, primVec);
	printf("Total polys in BVH tree: %u \n", numPolysBVH);
	
}

void tPolyShape::addPolysToBVH(tBVH &node, tAABB &nodeBox,std::vector<tPolygon*> &primVec) {
	if (primVec.size() > node.maxPrimsInNode) {
		//spawn a couple of new children at put primVec into them
		vec3 max,min;
		min = nodeBox.cen;
		tBVH *o1 = new tBVH(node.depth+1);	//+x+y+z
		max = nodeBox.cen + vec3(nodeBox.x_width/2.0, nodeBox.y_height/2.0, nodeBox.z_depth/2.0);
		o1->BV = new tAABB(min,max);
		node.children.push_back(o1);

		tBVH *o2 = new tBVH(node.depth+1);	//-x+y+z
		min = nodeBox.cen - vec3(nodeBox.x_width/2.0, 0, 0);
		max = nodeBox.cen + vec3(0, nodeBox.y_height/2.0, nodeBox.z_depth/2.0);
		o2->BV = new tAABB(min,max);
		node.children.push_back(o2);

		tBVH *o3 = new tBVH(node.depth+1);	//-x+y-z
		min = nodeBox.cen - vec3(nodeBox.x_width/2.0, 0, nodeBox.z_depth/2.0);
		max = nodeBox.cen + vec3(0, nodeBox.y_height/2.0, 0);
		o3->BV = new tAABB(min,max);
		node.children.push_back(o3);

		tBVH *o4 = new tBVH(node.depth+1);	//+x+y-z
		min = nodeBox.cen - vec3(0, 0, nodeBox.z_depth/2.0);
		max = nodeBox.cen + vec3(nodeBox.x_width/2.0, nodeBox.y_height/2.0, 0);
		o4->BV = new tAABB(min,max);
		node.children.push_back(o4);

		tBVH *o5 = new tBVH(node.depth+1);	//+x-y+z
		min = nodeBox.cen - vec3(0, nodeBox.y_height/2.0, 0);
		max = nodeBox.cen + vec3(nodeBox.x_width/2.0, 0, nodeBox.z_depth/2.0);
		o5->BV = new tAABB(min,max);
		node.children.push_back(o5);

		tBVH *o6 = new tBVH(node.depth+1);	//-x-y+z
		min = nodeBox.cen - vec3(nodeBox.x_width/2.0, nodeBox.y_height/2.0, 0);
		max = nodeBox.cen + vec3(0, 0, nodeBox.z_depth/2.0);
		o6->BV = new tAABB(min,max);
		node.children.push_back(o6);

		tBVH *o7 = new tBVH(node.depth+1);	//-x-y-z
		min = nodeBox.cen - vec3(nodeBox.x_width/2.0, nodeBox.y_height/2.0, nodeBox.z_depth/2.0);
		max = nodeBox.cen;
		o7->BV = new tAABB(min,max);
		node.children.push_back(o7);

		tBVH *o8 = new tBVH(node.depth+1);	//+x-y-z
		min = nodeBox.cen - vec3(0, nodeBox.y_height/2.0, nodeBox.z_depth/2.0);
		max = nodeBox.cen + vec3(nodeBox.x_width/2.0, 0, 0);
		o8->BV = new tAABB(min,max);
		node.children.push_back(o8);

		std::vector<tPolygon*> *prims = new std::vector<tPolygon*>[node.children.size()];
		
		for (size_t i=0; i<primVec.size(); i++) {
			//test prim vs the vankers
			bool _added = false;
			for (size_t j=0; j<node.children.size(); j++) {
				if (node.children.at(j)->BV->polyIntersects(primVec.at(i))) {
					prims[j].push_back(primVec.at(i));
					_added = true;
				}
			}
			if (!_added) {
				printf("Polygon no %u at depth %u not added.", i, node.depth);
			}
		}
		//recursive call on children
		//if (node.depth > 0) {
			addPolysToBVH(*o1, *o1->BV, prims[0]);
			addPolysToBVH(*o2, *o2->BV, prims[1]);
			addPolysToBVH(*o3, *o3->BV, prims[2]);
			addPolysToBVH(*o4, *o4->BV, prims[3]);
			addPolysToBVH(*o5, *o5->BV, prims[4]);
			addPolysToBVH(*o6, *o6->BV, prims[5]);
			addPolysToBVH(*o7, *o7->BV, prims[6]);
			addPolysToBVH(*o8, *o8->BV, prims[7]);
		//}
		//else {
		//	addPolysToBVH(*o1, *o1->BV, prims[0]);
		//}
		delete[] prims;
	}
	else {
		//put all the buggars into the beggar
		std::vector<tPolygon*>::iterator primIt = primVec.begin();
		while (primIt != primVec.end()) {
			node.prims.push_back((*primIt)->clone());
			primIt++;
		}
		numPolysBVH += node.prims.size();
		printf("Leaf at %u with %u polys.\n", node.depth, node.prims.size());
	}		 
}
//build BSP
void tPolyShape::buildBSP() {
	//iterate polygons
	std::vector<tPolygon>::iterator polyIt =
		this->mPolygonArray.begin();
	std::list<tPolygon> root_list;
	while (polyIt != mPolygonArray.end()) {
		root_list.push_back((*polyIt));
		polyIt++;
	}//end polyIt

	//choose plane
	//start with one trhough the middle of the object xz plane
	planes[0].a = 0;
	planes[0].b = 1;
	planes[0].c = 0;
	planes[0].d = 0;
	planes[1].a = 0;
	planes[1].b = 0;
	planes[1].c = 1;
	planes[1].d = 0;
	planes[2].a = 1;
	planes[2].b = 0;
	planes[2].c = 0;
	planes[2].d = 0;
	mPolygonTree.plane = planes[0];
	//add to tree
	numPolysBSP = 0;
	addPolysToTree(&mPolygonTree, root_list, 12);
	printf("Total polys in BSP tree: %u \n", numPolysBSP);
}
void tPolyShape::addPolysToTree(tBSPtree *tree, std::list<tPolygon> &polyList, unsigned int maxDepth) {
	if (polyList.size() > 10 ) {
		std::list<tPolygon> front_list, back_list;
		while (polyList.size() > 0) {
			tPolygon p = polyList.front();
			polyList.pop_front();

			if (!tree->front) {
				tree->front = new tBSPtree(tree->depth+1);
			}
			if (!tree->back) {
				tree->back = new tBSPtree(tree->depth+1);
			}
			//choose front or back
			int classification = tree->classify(p);
			switch (classification) {
				case 2:
				case 1:	//in front
					front_list.push_back(p);
					break;
				case -1: // behind
				case -2:
					back_list.push_back(p);
					break;
				case 0:	
				case -10:
				default:
					//put in both
					front_list.push_back(p);
					back_list.push_back(p);
					break;
			};
			
		}
		if (front_list.size() > 0) {
			//choose plane

			//find a center point for plane:
			std::list<tPolygon>::iterator pIt = front_list.begin();
			double4 center;
			center.x = center.y = center.z = 0;
			center.w = 1;
			unsigned int N = 0;//, i=0;
			//unsigned int Lsize =front_list.size()-1;
			//unsigned int chosen = rand()*Lsize/RAND_MAX;
			while (pIt != front_list.end()) {
				for (size_t v=0; v<(*pIt).vertices.size(); v++) {
					double4 *b = (*pIt).vertices.at(v);
					center.x += b->x;
					center.y += b->y;
					center.z += b->z;
					N++;
				}
				/*if (i == chosen) {
					tree->front->plane.planeFromVertices((*pIt).vertices.at(0), 
						(*pIt).vertices.at(1), 
						(*pIt).vertices.at(2));
					tree->front->plane.d -= SMALL_VAL*2;
				}
				i++;*/
				pIt++;
			}
			double one_over_N = 1.0/(double)N;
			center.x *= one_over_N;
			center.y *= one_over_N;
			center.z *= one_over_N;
			tree->front->plane = planes[tree->front->depth%3];
			tree->front->plane.d = -(tree->front->plane.distanceToPlane(center)-SMALL_VAL*2);
			addPolysToTree(tree->front, front_list,maxDepth); 
		}
		if (back_list.size() > 0) {
			//choose plane
			//find a center point for plane:
			std::list<tPolygon>::iterator pIt = back_list.begin();
			double4 center;
			center.x = center.y = center.z = 0;
			center.w = 1;
			unsigned int N = 0;//, i=0;
			//unsigned int Lsize = back_list.size()-1;
			//unsigned int chosen = rand()*Lsize/RAND_MAX;
			while (pIt != back_list.end()) {
				for (size_t v=0; v<(*pIt).vertices.size(); v++) {
					double4 *b = (*pIt).vertices.at(v);
					center.x += b->x;
					center.y += b->y;
					center.z += b->z;
					N++;
				}
				/*if (i == chosen) {
					tree->back->plane.planeFromVertices((*pIt).vertices.at(0), 
						(*pIt).vertices.at(1), 
						(*pIt).vertices.at(2));
					tree->back->plane.d -= SMALL_VAL*2;
				}
				i++;*/
				pIt++;
			}
			double one_over_N = 1.0/(double)N;
			center.x *= one_over_N;
			center.y *= one_over_N;
			center.z *= one_over_N;
			tree->back->plane = planes[tree->back->depth%3];
			tree->back->plane.d = -(tree->back->plane.distanceToPlane(center)-SMALL_VAL*2);

			addPolysToTree(tree->back, back_list,maxDepth);
		}
	}
	else {
		//set tree  shapes to this list
		tree->shapes =  polyList;
		numPolysBSP += tree->shapes.size();
		printf("Leaf at %u with %u polys.\n", tree->depth, tree->shapes.size());
	}
}


/*
	Returns the closest intersection of the ray
	with this tShape object. Returns -1 if no intersection found.
*/
double tPolyShape::intersection(const tRay &ray, tIntersection &hit){
	vec3 tmp;
	double closestHit = LARGE_VAL;
	bool _hit = false;
	tIntersection tmpHit;
	hit.surfaceIndex = 0;
	
	if ( boundingSphere.rayIntersects(ray)) {
		std::vector<tPolygon>::iterator pIt;
		size_t i = 0;
		
		/* BSP intersections
		double t;
		t = mPolygonTree.intersection(ray, tmpHit);		

		if (t > 0) {
			closestHit = t;
			hit = tmpHit;
			_hit = true;
		}*/

		// BVH intersections		
		double t;
		t = this->mBVH.intersection(ray, tmpHit);
		if (t>0) {
			closestHit = t;
			hit = tmpHit;
			_hit = true;
		}
		
	}
	
	if (_hit) {
		return closestHit;
	}
	else
		return -1.0;
}

