#include "painter.h"



Scene::Scene(int x, int y, int w, int h)
{
    this->w = w;
    this->h = h;
    pool_tex = new Texture(":/pool/pool.bmp");
    zbuf = new int[w * h];
    ibuf = Matrix(w*h, 3);
    ibuf_transparent = Matrix(w*h, 3);
    c_tr = new double[w*h];

    memset(zbuf, 0, sizeof(int) * w * h);
    memset(&ibuf[0][0], 0, 3 * w * h * sizeof(double));
    memset(&ibuf_transparent[0][0], 0, 3 * w * h * sizeof(double));
    memset(c_tr, 0, sizeof(double) * w * h);

    scene = new QGraphicsScene();
    scene->setSceneRect(x, y, w, h);

    img = new ImageItem(w, h);
    setViewPort(x, y, w, h);

    skybox = new Skybox();

    model = WaterModel(70);
    pool_model = PoolModel(model.surf[model.xvert+1][0],
            model.surf[model.xvert*(model.xvert-1)-1][0],
            model.surf[1][2],
            model.surf[model.xvert-2][2]);

    cam.eye[0] = -1.3;
    cam.eye[1] = -1.3;
    cam.eye[2] = 1.3;
    cam.eye_s = cam.eye;

    setviewmatr();
    setprojmatr();

    light_dir = Vector(3);
    light_dir[0] = -.9;
    light_dir[1] = -.9;
    light_dir[2] = .9;
    normalize(light_dir);

    scene->addItem(img);

    light_color = Vector(3);
    light_color = 255;

}

Scene::~Scene()
{
    delete img;
    delete scene;
    delete c_tr;
    delete zbuf;
}

void Scene::setViewPort(int x, int y, int w, int h)
{
    viewPort = Matrix(4, 4);
    identity4(viewPort);
    viewPort[0][3] = x+w/2.f;
    viewPort[1][3] = y+h/2.f;

    viewPort[2][3] = 1000/2.f;

    viewPort[0][0] = w/2.f;
    viewPort[1][1] = h/2.f;
    viewPort[2][2] = 1000/2.f;
}

void Scene::setviewmatr()
{
    cam.center[0] = 0;
    cam.center[1] = 0;
    cam.center[2] = 0;
    cam.up[0] = 0;
    cam.up[1] = 1;
    cam.up[2] = 0;

    Vector z(3);
    Vector x(3);
    Vector y(3);

    z = cam.eye - cam.center;
    normalize(z);
    x = cross(cam.up,z);
    normalize(x);
    y = cross(z,x);
    normalize(y);

    Matrix Minv(4, 4);
    Matrix Tr(4, 4);
    identity4(Minv);
    identity4(Tr);
    for (int i = 0; i < 3; i++)
    {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -cam.center[i];
    }
    viewMatr = Minv * Tr;
}

void Scene::setprojmatr()
{
    projMatr = Matrix(4, 4);
    identity4(projMatr);
    projMatr[3][2] = -1.0 /
            sqrt((cam.eye[0]) * (cam.eye[0]) + (cam.eye[1]) * (cam.eye[1]) + (cam.eye[2]) * (cam.eye[2]));

}

void Scene::drawimage()
{
    Matrix m = viewPort * projMatr * viewMatr;

    model.setscreensurf(m);
    pool_model.setscreensurf(m);
    pool_model.setnormals();
    int xv = model.xvert;

    double t = model.transparent;

        // Стенки бассейна - рисуются, если изменилось положение камеры
    if (changed)
    {
        changed = false;
        memset(zbuf, 0, sizeof(int) * w * h);
        memset(&ibuf[0][0], 0, 3 * w * h * sizeof(double));

        skybox->moveto(cam.eye);

        Matrix m1 = viewMatr;
        skybox->setscreensurf(m1);
        skybox->clip(cam);

        m1 = viewPort;
        skybox->setclipview(m1);

        int num = skybox->polygons.size();

       /* std::cout << "clip_polys.size = " << num << std::endl;*/
 std::cout << "START";
  //      #pragma omp parallel for
        for (int i = 0; i < num; i++)
        {
                triangle(skybox->vert(i), V(pool_model.i[4]), skybox->polygons[i], skybox->tex, true);


                std::cout << "i = " << i << std::endl;
                PolyVecs b = skybox->vert(i);
                std::cout << "A" << b.a[0] << " " << b.a[1] << " " << b.a[2] << std::endl;
                std::cout << "B" << b.b[0] << " " << b.b[1] << " " << b.b[2] << std::endl;
                std::cout << "C" << b.c[0] << " " << b.c[1] << " " << b.c[2] << std::endl;
                std::cout << std::endl << std::endl;
        }

        #pragma omp parallel for
        for (int i = 0; i < 4; i++)
        {
            if (pool_model.surf_deform_norms[2*i][2] > 0)
            {
                triangle(pool_model.vert(2*i), V(pool_model.i[4]), pool_model.polygons[2*i], pool_tex);
                triangle(pool_model.vert(2*i+1), V(pool_model.i[4]), pool_model.polygons[2*i+1], pool_tex);
            }
        }
        triangle(pool_model.vert(8), V(pool_model.i[4]), pool_model.polygons[8], pool_tex);
        triangle(pool_model.vert(9), V(pool_model.i[4]), pool_model.polygons[9], pool_tex);
    }

    // Поверхность воды
    int num = model.polygons.size();
    #pragma omp parallel for
    for (int i = 0; i < num; i++)
    {
        triangle(model.vert(i), model.intencity(i), t);
    }

    PolyI iwall;
    iwall.ia = model.i_wall;
    iwall.ib = iwall.ia;
    iwall.ic = iwall.ia;

    // Стенки воды
    #pragma omp parallel for
    for (int i = 0; i < 7; i += 2)
    {
        if (pool_model.surf_deform_norms[6-i][2] <= 0)
        {
            for(int l = i*(xv-3); l < (i+2)*(xv - 3); l++)
            {
                triangle(model.wallvert(l), iwall, t);
            }
        }
    }
    // Объединение буферов и вывод на изображение
    #pragma omp parallel for
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            int k = i*w + j;
            img->set(j, i, ibuf_transparent[k][0] + (1 - c_tr[k]) * ibuf[k][0]+5,
                ibuf_transparent[k][1] + (1 - c_tr[k]) * ibuf[k][2]+5,
                ibuf_transparent[k][1] + (1 - c_tr[k]) * ibuf[k][2]+5);
        }
    memset(&ibuf_transparent[0][0], 0, 3 * w * h * sizeof(double));
    memset(c_tr, 0, sizeof(double) * w * h);
}

void Scene::updheights()
{
    model.updheights();
}

void Scene::rand_disturb()
{
    int k = rand() % (model.xvert * model.xvert - model.xvert);
    if ((k+1) % model.xvert == 0)
        k--;

    model.H1[k] += .1;
    model.H2[k] += .1;

    model.H1[k + 1] += .1;
    model.H2[k + 1] += .1;

    model.H1[k + model.xvert] += .1;
    model.H2[k + model.xvert] += .1;

    model.H1[k + model.xvert + 1] += .1;
    model.H2[k + model.xvert + 1] += .1;
}

void Scene::calc_normals()
{
    model.calc_normals();
    pool_model.calc_normals();

}

void Scene::calc_intencities()
{
    Vector v = norm(cam.eye);
    Vector h = norm(v + light_dir);

    int xvert = model.xvert;

    #pragma omp parallel for
    for (int i = 1; i < xvert-1; ++i)
    {
        for (int j = 1; j < xvert-1; ++j)
        {
            int k = i * xvert + j;
            // Фоновое освещение
            model.i[k] = model.color * 0.1;
            // Диффузное освещение (по Ламберту)
            model.i[k] += 0.7 * model.color * std::max(dot(light_dir, norm(V(model.vert_norms[k]))), 0.0);
            // Зеркальное освещение (по Блинну-Фонгу)
            model.i[k] += 0.2 * light_color * pow(std::max(0.0, dot(h, norm(V(model.vert_norms[k])))), 20);
        }
    }

    xvert = pool_model.xvert;
    #pragma omp parallel for
    for (int i = 0; i < 5; ++i)
    {
            // Фоновое освещение
            pool_model.i[i] = pool_model.color * 0.6;
            // Диффузное освещение (по Ламберту)
            pool_model.i[i] += 0.9 * pool_model.color * std::max(dot(light_dir, norm(V(pool_model.surf_norms[2*i]))), 0.0);
    }
}

void Scene::set_changed()
{
    changed = true;
}

void Scene::triangle(const struct PolyVecs &p, Vector i_, TrPolygon uv, Texture *tex, bool usez)
{
    double u[] = {uv.u[0], uv.u[1], uv.u[2]};
    double v[] = {uv.v[0], uv.v[1], uv.v[2]};

    const Vector *a = &(p.a);
    const Vector *b = &(p.b);
    const Vector *c = &(p.c);
    const Vector *tv;


    i_ = i_ / 255.0;
    int x, y;                                                   // координаты точки в скан-линии
    int length, len;                                            // длина отрезка, оставшаяся длина спана
                                                                // текущие координаты в текстуре
    float du, dv;                                               // на сколько изменятся координаты в текстуре
                                                                // при изменении x на 1
    float duz, dvz, dwz;                                        // дельты интерполируемых величин
    float wz1, uz1, vz1, wz2, uz2, vz2, uz3, vz3, wz3;          // интерполируемые величины, переменные
                                                                // для сортировки
    float uz_a, vz_a, wz_a, uz_b, vz_b, wz_b;                   // переменные для начала и конца спана
    float u_a, v_a, u_b, v_b;                                   // координаты в текстуре левой и правой сторон
                                                                // спана
    float x1, x2, x3;
    float z1, z2, z3;
    double t;

    float z, dz;

    // Сортировка по Y
    if ((*b)[1] < (*a)[1]) {
        tv = a; a = b; b = tv;
        t = u[0]; u[0] = u[1]; u[1] = t;
        t = v[0]; v[0] = v[1]; v[1] = t;
    }
    if ((*c)[1] < (*a)[1]) {
        tv = a; a = c; c = tv;
        t = u[0]; u[0] = u[2]; u[2] = t;
        t = v[0]; v[0] = v[2]; v[2] = t;
    }
    if ((*b)[1] > (*c)[1]) {
        tv = c; c = b; b = tv;
        t = u[1]; u[1] = u[2]; u[2] = t;
        t = v[1]; v[1] = v[2]; v[2] = t;
    }

    if ((int)(*a)[1] == (int)(*c)[1])
        return;

    double awz = 1 / (*a)[3];                                         // подсчитываем 1/z, u/z, v/z для каждой вершины
    double auz = u[0] * awz;
    double avz = v[0] * awz;
    double bwz = 1 / (*b)[3];
    double buz = u[1] * bwz;
    double bvz = v[1] * bwz;
    double cwz = 1 / (*c)[3];
    double cuz = u[2] * cwz;
    double cvz = v[2] * cwz;

    t = ((*b)[1] - (*a)[1]) / ((*c)[1] - (*a)[1]);
    x1 = (*a)[0] + ((*c)[0] - (*a)[0]) * t; // получаем x-координату начала отрезка,
    wz1 = awz + (cwz - awz) * t;
    uz1 = auz + (cuz - auz) * t;
    vz1 = avz + (cvz - avz) * t;
    z1 = (*a)[2] + ((*c)[2] - (*a)[2]) * t;

    x2 = (*b)[0];
    uz2 = buz;
    vz2 = bvz;
    wz2 = bwz;
    duz = (uz1 - uz2) / (x1 - x2);                             // получаем дельты
    dvz = (vz1 - vz2) / (x1 - x2);
    dwz = (wz1 - wz2) / (x1 - x2);
    z2 = (*b)[2];

    for (y = (int)(*a)[1]+1; y <= (int)(*c)[1]; y++)                          // цикл отрисовки грани
    {
        if (y < 0 || y >= this->h)
            continue;


        float u, v;
        t = (y - (*a)[1]) / ((*c)[1] - (*a)[1]);
        x1 = (*a)[0] + ((*c)[0] - (*a)[0]) * t;       // получаем x-координату самой длинной стороны
        uz1 = auz + (cuz - auz) * t;   // получаем uz-координату
        vz1 = avz + (cvz - avz) * t;   // получаем vz-координату
        wz1 = awz + (cwz - awz) * t;   // получаем wz-координату
        z1 = (*a)[2] + ((*c)[2] - (*a)[2]) * t;

        if (y >= (*b)[1])
        {                                          // выясняем, какую сторону проходит скан-линия,
            t = (y - (*b)[1]) / ((*c)[1] - (*b)[1]);
            x2 = (*b)[0] + ((*c)[0] - (*b)[0]) * t; // и вычисляем ее x-координату
            uz2 = buz + (cuz - buz) * t; //            uz-координату
            vz2 = bvz + (cvz - bvz) * t; //            vz-координату
            wz2 = bwz + (cwz - bwz) * t; //            wz-координату
            z2 = (*b)[2] + ((*c)[2] - (*b)[2]) * t;
        }
        else
        {
            t = (y - (*a)[1]) / ((*b)[1] - (*a)[1]);
            x2 = (*a)[0] + ((*b)[0] - (*a)[0]) * t;
            uz2 = auz + (buz - auz) * t;
            vz2 = avz + (bvz - avz) * t;
            wz2 = awz + (bwz - awz) * t;
            z2 = (*a)[2] + ((*b)[2] - (*a)[2]) * t;
        }

         if (x1 > x2)
         {
             z3 = z1; z1 = z2; z2 = z3;
             x3 = x1; x1 = x2; x2 = x3;
             uz3 = uz1; uz1 = uz2; uz2 = uz3;
             vz3 = vz1; vz1 = vz2; vz2 = vz3;
             wz3 = wz1; wz1 = wz2; wz2 = wz3;
         }
         x = x1;
         z = z1;
         length = (int)x2 - (int)x1;                               // вычисляем длину всего отрезка
         uz_a = uz1;                                               // подготавливаем переменные для спана и
         vz_a = vz1;                                               // вычисляем u, v на начало спана
         wz_a = wz1;
         u_a = uz_a / wz_a;
         v_a = vz_a / wz_a;
         dz = (z2 - z1) / (x2 - x1);
         while (length >= 8) {
            uz_b = uz_a + 8 * duz;                                  // подготавливаем переменные для конца текущего
            vz_b = vz_a + 8 * dvz;                                  // спана и вычисляем u, v на конец спана
            wz_b = wz_a + 8 * dwz;
            u_b = uz_b / wz_b;
            v_b = vz_b / wz_b;

            u = u_a;
            v = v_a;

            du = (u_b - u_a) / 8;                                   // вычисляем du и dv на спан
            dv = (v_b - v_a) / 8;

            len = 8;
            while (len--)
            {
                int idx = y * w + x;
                if ((x > 0) && (x < this->w))
                    if ((!usez) || (zbuf[idx] < z))
                    {
                        QRgb i = tex->texel(u, v);

                        ibuf[idx][0] = qRed(i) * i_[0];
                        ibuf[idx][1] = qGreen(i) * i_[1];
                        ibuf[idx][2] = qBlue(i) * i_[2];
                        if (usez)
                            zbuf[idx] = z;
                    }
                z += dz;
                x++;
                u += du;
                v += dv;
            }
            length -= 8;
            uz_a = uz_b;
            vz_a = vz_b;
            wz_a = wz_b;
            u_a = u_b;
            v_a = v_b;
         }

         if (length > 0)
         {                                        // дорисовываем оставшиеся пиксели
              uz_b = uz2;
              vz_b = vz2;
              wz_b = wz2;
              u_b = uz_b / wz_b;
              v_b = vz_b / wz_b;
              u = u_a;
              v = v_a;
              du = (u_b - u_a) / length;
              dv = (v_b - v_a) / length;

              while (length--)
              {
                  int idx = y * w + x;
                  if (x > 0 && x < this->w)
                      if ((!usez) || (zbuf[idx] < z))
                      {
                          QRgb i = tex->texel(u, v);
                          ibuf[idx][0] = qRed(i) * i_[0];
                          ibuf[idx][1] = qGreen(i) * i_[1];
                          ibuf[idx][2] = qBlue(i) * i_[2];
                          if (usez)
                            zbuf[idx] = z;
                      }
                    z += dz;
                    x++;
                    u += du;
                    v += dv;
              }
         }
    }
}


void Scene::triangle(const struct PolyVecs &p, struct PolyI i, double c_)
{
    const Vector *a = &(p.a);
    const Vector *b = &(p.b);
    const Vector *c = &(p.c);
    const Vector *t;


    if ((*b)[1] < (*a)[1]) {
        t = a; a = b; b = t;
        i.ia.swap(i.ib);
    }
    if ((*c)[1] < (*a)[1]) {
        t = c; c = a; a = t;
        i.ia.swap(i.ic);
    }
    if ((*b)[1] > (*c)[1]) {
        t = c; c = b; b = t;
        i.ib.swap(i.ic);
    }
    if (int((*a)[1]+.5) == int((*b)[1]+.5) && (*b)[0] < (*a)[0])
    {
        t = a; a = b; b = t;
        i.ia.swap(i.ib);
    }

    int x1 = int((*a)[0] + .5);
    int x2 = int((*b)[0] + .5);
    int x3 = int((*c)[0] + .5);
    int y1 = int((*a)[1] + .5);
    int y2 = int((*b)[1] + .5);
    int y3 = int((*c)[1] + .5);
    int z1 = int((*a)[2] + .5);
    int z2 = int((*b)[2] + .5);
    int z3 = int((*c)[2] + .5);

    double dx13 = 0, dx12 = 0, dx23 = 0;
    double dz13 = 0, dz12 = 0, dz23 = 0;
    Vector di13(3);
    Vector di12(3);
    Vector di23(3);


    if (y3 != y1)
    {
        dz13 = (z3 - z1) / (double)(y3 - y1);
        dx13 = (x3 - x1) / (double)(y3 - y1);
        di13 = (i.ic - i.ia) / (double)(y3 - y1);
    }
    else
    {
        dx13 = 0;
        dz13 = 0;
        di13 = 0;
    }
    if (y2 != y1)
    {
        dz12 = (z2 - z1) / (double)(y2 - y1);
        dx12 = (x2 - x1) / (double)(y2 - y1);
        di12 = (i.ib - i.ia) / (double)(y2 - y1);
    }
    else
    {
        dz12 = 0;
        dx12 = 0;
        di12 = 0;
    }
    if (y3 != y2)
    {
        dz23 = (z3 - z2) / (double)(y3 - y2);
        dx23 = (x3 - x2) / (double)(y3 - y2);
        di23 = (i.ic - i.ib) / (double)(y3 - y2);
    }
    else
    {
        dz23 = 0;
        dx23 = 0;
        di23 = 0;
    }
    double z;
    double dz;

    Vector iv(3);
    Vector di(3);

    double wx1 = x1;
    double wx2 = wx1;
    double wz1 = z1;
    double wz2 = z1;
    Vector wi1(i.ia);
    Vector wi2(i.ia);

    Vector _di13(di13);
    double _dx13 = dx13;
    double _dz13 = dz13;

    if (dx13 > dx12)
    {
        swap_(&dz13, &dz12);
        swap_(&dx13, &dx12);
        di13.swap(di12);
    }
    for (int i = y1; i < y2; i++)
    {
        z = wz1;
        iv = wi1;

        if (wx1 != wx2)
        {
            dz = (wz2 - wz1) / (double)(wx2 - wx1);
            di = (wi2 - wi1) / (double)(wx2 - wx1);
        }
        else
        {
            dz = 0;
            di = 0;
        }
        for (int j = wx1; j < wx2; j++)
        {
            int idx = i * h + j;

            if (zbuf[idx] < z)
            {
                if (iv[2] > ibuf_transparent[idx][2])
                {
                    c_tr[idx] = c_;
                    ibuf_transparent[idx] = c_ * iv;
                }
            }
            z += dz;
            iv += di;
        }
        wx1 += dx13;
        wx2 += dx12;
        wz1 += dz13;
        wz2 += dz12;
        wi1 += di13;
        wi2 += di12;
    }

    if (y1 == y2){
        wx1 = x1;
        wx2 = x2;
        wz1 = z1;
        wz2 = z2;
        wi1 = i.ia;
        wi2 = i.ib;
    }
    if (_dx13 < dx23)
    {
        swap_(&_dx13, &dx23);
        swap_(&_dz13, &dz23);
        _di13.swap(di23);
    }
    for (int i = y2; i < y3; i++){
        z = wz1;
        iv = wi1;

        if (wx1 != wx2)
        {
            dz = (wz2 - wz1) / (double)(wx2 - wx1);
            di = (wi2 - wi1) / (double)(wx2 - wx1);
        }
        else
        {
            dz = 0;
            di = 0;
        }

        for (int j = wx1; j < wx2; j++)
        {
            int idx = i * h + j;
           if (zbuf[idx] < z)
            {
               if (iv[2] > ibuf_transparent[idx][2])
               {
                   c_tr[idx] = c_;
                   ibuf_transparent[idx] = c_ * iv;
               }
            }
            z += dz;
            iv += di;
        }
        wx1 += _dx13;
        wx2 += dx23;
        wz1 += _dz13;
        wz2 += dz23;
        wi1 += _di13;
        wi2 += di23;
    }
}

