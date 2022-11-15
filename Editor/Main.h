void MyStart()
{
Cube cube;
DirectionalLight light;
Plane plane;
plane.material.ambient = {1.000000f,1.000000f,1.000000f,1.000000f};
plane.material.diffuse = {1.000000f,1.000000f,1.000000f,1.000000f};
plane.material.specular = {0.000000f,0.000000f,0.000000f,1.000000f};
plane.position = {0.000000f,0.000000f,0.000000f};
plane.rotation = {0.000000f,0.000000f,0.000000f};
plane.scale = {10.000000f,1.000000f,10.000000f};
plane.lighting = 1;
ObjectManager::Add(plane);
plane.material.ambient = {1.000000f,1.000000f,1.000000f,1.000000f};
plane.material.diffuse = {1.000000f,1.000000f,1.000000f,1.000000f};
plane.material.specular = {0.000000f,0.000000f,0.000000f,1.000000f};
plane.position = {20.000000f,0.000000f,0.000000f};
plane.rotation = {0.000000f,0.000000f,0.000000f};
plane.scale = {10.000000f,1.000000f,10.000000f};
plane.lighting = 1;
ObjectManager::Add(plane);
Camera::SetPosition({0.000000f,0.000000f,0.000000f});
Camera::SetRotation({0.000000f,0.000000f,0.000000f});
}
