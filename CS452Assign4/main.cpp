#include "main.h"

#include "CS148/Scene.h"

using namespace std;

//****************************************************
// Global Variables
//****************************************************
Viewport * viewport;
mat4 viewToWorld;
World * world;
Film * film;
Scene * scene;

//-------------------------------------------------------------------------------
RGB getSurfaceTransmittance(Primitive *p, Ray &ray, vec4 intersection) {
	// apply Beer's law
	RGB transparency_color = p->getColor() * p->getMaterial().getMT();
	return RGB(expf(-transparency_color[RED]), expf(-transparency_color[GREEN]), expf(-transparency_color[BLUE]));
}

//-------------------------------------------------------------------------------
// Here you raycast a single ray, calculating the color of this ray.
RGB traceRay(Ray & ray, int depth) {
    RGB retColor(0,0,0);

	//Use the "world" global variable to access the primitives and lights in the input file.
	// write methods on your primitives and lights to help you.	
	Primitive *p;
	double t_shift = 0.001;

	//Please be sure to read the following classes in algebra3.h:
	// - Color
	// - Point (useful for storing sample information)
	// - Material
	// - Ray

	//IMPORTANT:
	//Please start all bounce rays at a t value of 0.1 - this has the effect of slightly offsetting
	//bounce rays from the surface they're bouncing from, and prevents bounce rays from being occluded by their own surface.	

	vec3 end = ray.getPos(ray.getMinT() + t_shift);
	Ray ray_shifted = Ray(vec3(ray.start()), end, ray.getMinT() + t_shift);
	p = world->intersect(ray_shifted, t_shift);

	vec4 new_intersect = ray.getPos(t_shift);

	if (p != NULL)
	{
		
		//retColor = RGB(1, 1, 1);
		RGB amb = (world->getAmbientLightColor())*(p->getColor())*(p->getMaterial().getMA());
		retColor += amb;
		for (vector<Light*>::const_iterator i = world->getLightsBeginIterator(); i != world->getLightsEndIterator(); ++i) {
			Light *temp_light = *i;
			//bool bleh = TRUE;
			//Ray shadow_ray(temp_light->getShadowRay(new_intersect, bleh));
			//Primitive *shadow_test = world->intersect(shadow_ray, t_shift);
			//if (shadow_test == NULL) {
				vec4 norm = (p->calculateNormal(new_intersect)).normalize();
				vec4 light_dir;
				temp_light->getIncidenceVector(new_intersect, light_dir);
				double prod = norm*light_dir;
				if (prod <= 0) {
					prod = 0;
				}
				RGB l_val = (p->getMaterial().getML())*(p->getColor())*(temp_light->getColor(new_intersect))*prod;
				retColor += l_val;

				vec4 ref_vector = -light_dir + 2 * (light_dir*norm)*norm;
				RGB highlight = (p->getMaterial().getMSM())*(p->getColor()) + (1 - p->getMaterial().getMSM())*(temp_light->getColor(new_intersect));
				vec4 u = viewport->getViewVector(new_intersect);
				double prod_spec = (-ref_vector).normalize()*u;
				if (prod_spec <= 0) {
					prod_spec = 0;
				}
				RGB s_val = (p->getMaterial().getMS())*highlight*(temp_light->getColor(new_intersect))*(pow(prod_spec, p->getMaterial().getMSP()));
				retColor += s_val;
			//}
		}
		if (depth <= MAX_DEPTH) {
			vec4 norm = p->calculateNormal(new_intersect);
			vec4 ray_inc = ray_shifted.direction();
			vec4 ref_ray = ray_inc - 2 * (ray_inc*norm)*norm;
			vec4 end_pos = new_intersect + ref_ray;
			Ray reflected(vec3(new_intersect), vec3(end_pos), 0.01);
			RGB rcol = traceRay(reflected, depth + 1);
			RGB reflectcol = p->getMaterial().getMR()*rcol;
			retColor += reflectcol;
		}
	}
	
    return retColor;
}

//-------------------------------------------------------------------------------
// Here you write your main render function. This calls the raycast method
// on each ray, which you generate using the Sampler and Ray classes.
void renderWithRaycasting() {
    Sample sample;    //This is the point on the viewport being sampled.
    Ray ray;        //This is the ray being traced from the eye through the point.
    RGB c(0,0,0);

    viewport->resetSampler();
    while(viewport->getSample(sample)) {  //just gets a 2d sample position on the film
        c = RGB(0,0,0);
    	ray = viewport->createViewingRay(sample);  //convert the 2d sample position to a 3d ray
        ray.transform(viewToWorld);  //transform this to world space
        c += traceRay(ray, 0);
        film->expose(c, sample);
    }
	film->bakeAndSave();
	cout << "Image saved!" << endl;
}

//-------------------------------------------------------------------------------
// This traverses the read-in scene file's DAG and builds a list of
// primitives, lights and the viewport object. See World.h
void sceneToWorld(SceneInstance *inst, mat4 localToWorld, int time) {
    if (inst == NULL)
        return;

    mat4 nodeXform;
    inst->computeTransform(nodeXform,time);
    localToWorld = localToWorld * nodeXform;

    SceneGroup *g = inst->getChild();
    if (g == NULL) { // for example if the whole scene fails to load
        cout << "ERROR: We arrived at an instance with no child?!" << endl;
        return;
    }

    int ccount = g->getChildCount();
    for (int i = 0; i < ccount; i++) {
        sceneToWorld(g->getChild(i), localToWorld, 0);
    }

    CameraInfo f;
    if (g->computeCamera(f, time)) {
        viewToWorld = localToWorld;

        if (viewport != NULL)
            delete viewport;

        vec4 eye(0.0, 0.0, 0.0, 1.0);
        vec4 LL(f.sides[FRUS_LEFT], f.sides[FRUS_BOTTOM], -f.sides[FRUS_NEAR], 1.0);
        vec4 UL(f.sides[FRUS_LEFT], f.sides[FRUS_TOP], -f.sides[FRUS_NEAR], 1.0);
        vec4 LR(f.sides[FRUS_RIGHT], f.sides[FRUS_BOTTOM], -f.sides[FRUS_NEAR], 1.0);
        vec4 UR(f.sides[FRUS_RIGHT], f.sides[FRUS_TOP], -f.sides[FRUS_NEAR], 1.0);

        viewport = new Viewport(eye, LL, UL, LR, UR, IMAGE_WIDTH, IMAGE_HEIGHT);
    }

    LightInfo l;
    if (g->computeLight(l, time)) {
        if (l.type == LIGHT_AMBIENT) {
            RGB amb = world->getAmbientLightColor();
            world->setAmbientLightColor(amb + l.color);
        }
        else if (l.type == LIGHT_DIRECTIONAL) {
            DirectionalLight *li = new DirectionalLight(l.color);
            vec4 dir(0,0,-1,0);
            li->setDirection(localToWorld*dir);
            world->addLight(li);
        }
        else if (l.type == LIGHT_POINT) {
            PointLight *li = new PointLight(l.color, l.falloff, l.deadDistance);
            vec4 pos(0,0,0,1);
            li->setPosition(localToWorld*pos);
            world->addLight(li);
        }
        else if (l.type == LIGHT_SPOT) {
            throw "oh no";
        }
    }

    double r;
    MaterialInfo m;
    if (g->computeSphere(r, m, time)) {
        Material mat(m.k[0],m.k[1],m.k[2],m.k[3],m.k[4],m.k[MAT_MS],m.k[5],m.k[6]);
        Sphere *sph = new Sphere(r, m.color, mat, localToWorld);
        world->addPrimitive(sph);
    }

    OBJTriangleMesh *t;
    if (g->computeMesh(t, m, time)) {
        Material mat(m.k[0],m.k[1],m.k[2],m.k[3],m.k[4],m.k[MAT_MS],m.k[5],m.k[6]);
        for (vector<OBJTriangle*>::iterator it = t->triangles.begin(); it != t->triangles.end(); ++it) {
            Triangle *tri = new Triangle(
                                t->vertices[ (**it).ind[0] ]->pos,
                                t->vertices[ (**it).ind[1] ]->pos,
                                t->vertices[ (**it).ind[2] ]->pos,
                                m.color, mat, localToWorld);
            world->addPrimitive(tri);
        }
    }

}

//-------------------------------------------------------------------------------
/// Initialize the environment
int main(int argc,char** argv) {
	
	if (argc != 3) {
		cout << "USAGE: raytracer scene.scd output.png" << endl;
		exit(1);
	}
	
	film = new Film(IMAGE_WIDTH, IMAGE_HEIGHT, argv[2]);
    scene = new Scene(argv[1]);
    viewToWorld = identity3D();
    world = new World();
    sceneToWorld(scene->getRoot(), identity3D(), 0);
	cout << "Imported Scene File." << endl;
	world->printStats();
	renderWithRaycasting();
}
