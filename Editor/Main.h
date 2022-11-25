void MyStart()
{
Cube cube;
DirectionalLight dir;
PointLight point;
Model model;
Plane plane;
point.att = {0.000000,0.100000,0.000000};
point.ambient = {0.000000,0.000000,0.000000,1.000000};
point.diffuse = {0.500000,0.200000,0.100000,1.000000};
point.position = {0.000000,20.000000,10.000000};
point.range = 500.000000;
point.specular = {1.000000,1.000000,1.000000,50.000000};
ObjectManager::Add(point);
model.name = L"FinalBaseMesh.obj";
model.material.ambient = {1.000000,1.000000,1.000000,1.000000};
model.material.diffuse = {1.000000,1.000000,1.000000,1.000000};
model.material.specular = {1.000000,1.000000,1.000000,50.000000};
model.position = {0.000000,0.000000,0.000000};
model.rotation = {0.000000,0.000000,0.000000};
model.scale = {1.000000,1.000000,1.000000};
model.lighting = 1;
ObjectManager::Add(model);
plane.material.ambient = {1.000000,1.000000,1.000000,1.000000};
plane.material.diffuse = {1.000000,1.000000,1.000000,1.000000};
plane.material.specular = {0.000000,0.000000,0.000000,1.000000};
plane.position = {0.000000,0.000000,0.000000};
plane.rotation = {0.000000,0.000000,0.000000};
plane.scale = {10.000000,1.000000,10.000000};
plane.lighting = 1;
ObjectManager::Add(plane);
Camera::SetPosition({3.113094,3.718466,-22.900904,0.000000});
Camera::SetRotation({-0.191000,-0.206000,0.000000,0.000000});
}
