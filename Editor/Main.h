void MyStart()
{
Cube cube;
DirectionalLight light;
Plane plane;
light.ambient = {0.000000f,0.000000f,0.000000f,0.000000f};
light.direction = {-0.500000f,-0.500000f,-0.500000f};
light.diffuse = {0.500000f,0.500000f,0.500000f,1.000000f};
light.specular = {1.000000f,1.000000f,1.000000f,50.000000f};
ObjectManager::Add(light);
plane.material.ambient = {1.000000f,1.000000f,1.000000f,1.000000f};
plane.material.diffuse = {1.000000f,1.000000f,1.000000f,1.000000f};
plane.material.specular = {0.000000f,0.000000f,0.000000f,1.000000f};
plane.position = {0.000000f,0.000000f,0.000000f};
plane.rotation = {0.000000f,0.000000f,0.000000f};
plane.scale = {100.000000f,100.000000f,100.000000f};
plane.lighting = 1;
ObjectManager::Add(plane);
cube.material.ambient = {1.000000f,1.000000f,1.000000f,1.000000f};
cube.material.diffuse = {1.000000f,1.000000f,1.000000f,1.000000f};
cube.material.specular = {0.000000f,0.000000f,0.000000f,1.000000f};
cube.position = {0.000000f,5.000000f,0.000000f};
cube.rotation = {0.000000f,0.000000f,0.000000f};
cube.scale = {0.500000f,0.500000f,0.500000f};
cube.lighting = 1;
ObjectManager::Add(cube);
cube.material.ambient = {1.000000f,1.000000f,1.000000f,1.000000f};
cube.material.diffuse = {1.000000f,1.000000f,1.000000f,1.000000f};
cube.material.specular = {0.000000f,0.000000f,0.000000f,1.000000f};
cube.position = {0.000000f,0.000000f,5.000000f};
cube.rotation = {0.000000f,0.000000f,0.000000f};
cube.scale = {0.500000f,0.500000f,0.500000f};
cube.lighting = 1;
ObjectManager::Add(cube);
Camera::SetPosition({0.000000f,0.000000f,0.000000f});
Camera::SetRotation({0.000000f,0.000000f,0.000000f});
}
