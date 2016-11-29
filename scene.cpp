#include "scene.h"



Scene::Scene(int x, int y, int w, int h)
{
    this->w = w;
    this->h = h;

    img = new ImageItem(w, h);

    scene = new QGraphicsScene();
    scene->setSceneRect(x, y, w, h);

    skybox = new Skybox();

    model = new WaterModel(70);
    model->tex = new ImageItem(w/3, h/3);

    pool_model = new PoolModel(model->surf[model->xvert+1][0],
            model->surf[model->xvert*(model->xvert-1)-1][0],
            model->surf[1][2],
            model->surf[model->xvert-2][2]);

    cam = new Camera(-.4, -.4, -.4);
    reflcam = new Camera(0, 0, 0);

    light_dir = Vector(3);
    light_dir[0] = -.9;
    light_dir[1] = -.9;
    light_dir[2] = .9;
    normalize(light_dir);

    scene->addItem(img);

    light_color = Vector(3);
    light_color = 255;

    renderer = new Renderer(w, h);
    refl_renderer = new Renderer(w/3, h/3);

    Matrix scale = Matrix(4, 4);
    identity4(scale);
    scale = scale / 1.6;
    scale[3][3] = 1;
    model->apply_matrix(scale);
    pool_model->apply_matrix(scale);

}

Scene::~Scene()
{
    delete this->reflcam;
    delete this->cam;
    delete this->pool_model;
    delete this->model;
    delete this->scene;
    delete this->img;
}

void Scene::render()
{
    if (changed)
    {
        changed = false;

        *this->reflcam = *this->cam;
        this->reflcam->eye[0] = this->reflcam->center[0] - this->cam->center[0] + this->cam->eye[0];
        this->reflcam->eye[1] = this->reflcam->center[1] + this->cam->center[1] - this->cam->eye[1];
        this->reflcam->eye[2] = this->reflcam->center[2] - this->cam->center[2] + this->cam->eye[2];

        refl_renderer->render(this->model->tex, this->model, this->pool_model, this->skybox, this->reflcam, 0.45, 1);

        this->model->rotateuv(-this->cam->anglex);

        renderer->render(this->img, this->model, this->pool_model, this->skybox, this->cam, 1, 7);
    }
    else
    {
        renderer->render(this->img, this->model, this->pool_model, this->skybox, this->cam, 1, 4);
    }
}

void Scene::updheights()
{
    model->updheights();
}

void Scene::rand_disturb()
{
    int k = rand() % (model->xvert * model->xvert - model->xvert);
    if ((k+1) % model->xvert == 0)
        k--;

    model->H1[k] += .05;
    model->H2[k] += .05;

    model->H1[k + 1] += .05;
    model->H2[k + 1] += .05;

    model->H1[k + model->xvert] += .05;
    model->H2[k + model->xvert] += .05;

    model->H1[k + model->xvert + 1] += .05;
    model->H2[k + model->xvert + 1] += .05;
}

void Scene::calc_normals()
{
    model->calc_normals();
    pool_model->calc_normals();
}

void Scene::calc_intencities()
{
    Vector v = norm(cam->eye);
    Vector h = norm(v + light_dir);

    int xvert = model->xvert;

    #pragma omp parallel for
    for (int i = 1; i < xvert-1; ++i)
    {
        for (int j = 1; j < xvert-1; ++j)
        {
            int k = i * xvert + j;
            // Фоновое освещение
            model->i[k] = model->color * 0.1;
            // Диффузное освещение (по Ламберту)
            model->i[k] += 0.7 * model->color * std::max(dot(light_dir, norm(V(model->vert_norms[k]))), 0.0);
            // Зеркальное освещение (по Блинну-Фонгу)
            model->i[k] += 0.2 * light_color * pow(std::max(0.0, dot(h, norm(V(model->vert_norms[k])))), 20);
        }
    }

    xvert = pool_model->xvert;
    #pragma omp parallel for
    for (int i = 0; i < 5; ++i)
    {
            // Фоновое освещение
            pool_model->i[i] = pool_model->color * 0.6;
            // Диффузное освещение (по Ламберту)
            pool_model->i[i] += 0.9 * pool_model->color * std::max(dot(light_dir, norm(V(pool_model->surf_norms[2*i]))), 0.0);
    }
}

void Scene::set_changed()
{
    changed = true;
}


