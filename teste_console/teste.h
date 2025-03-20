

#ifndef TESTE_H
#define TESTE_H

using namespace std;


namespace gpp {

void testAABB(){
    // Teste 1: Interseção entre AABBs
    AABB aabb1({0, 0, 0}, {2, 2, 2});
    AABB aabb2({1, 1, 1}, {3, 3, 3});

    cout << "AABB1:\n" << aabb1.toString() << "\n";
    cout << "AABB2:\n" << aabb2.toString() << "\n";

    if (aabb1.intersects(aabb2)) {
        cout << "AABB1 e AABB2 estão se intersectando!\n";
    } else {
        cout << "AABB1 e AABB2 NÃO estão se intersectando.\n";
    }

    // Teste 2: Raycasting
    RayInfo rayInfo({-1, 0.5, 0.5}, {1, 0, 0});
    if (aabb1.rayCast(&rayInfo)) {
        cout << "Ray intersectou a AABB1!\n";
        cout << rayInfo.toString() << "\n";
    } else {
        cout << "Ray NÃO intersectou a AABB1.\n";
    }

    // Teste 3: Adicionar AABBs
    AABB aabb3({-1, -1, -1}, {1, 1, 1});
    aabb3.addAABB(aabb1);

    cout << "AABB3 após adicionar AABB1:\n" << aabb3.toString() << "\n";

}

void testSphere() {
    // Cria uma esfera com raio 2.0 e centro na origem
    Sphere3d sphere(2.0f);

    // Teste 1: Verifica o método toString
    cout << " Teste 1: toString " << endl;
    cout << sphere.toString() << endl;

    // Teste 2: Verifica o método getClosestPoint
    cout << "\n Teste 2: getClosestPoint " << endl;
    vector3d pt(3.0f, 0.0f, 0.0f);  // Ponto fora da esfera
    vector3d closest = sphere.getClosestPoint(pt);
    cout << "Ponto mais próximo de " << pt << " é " << closest << endl;

    // Teste 3: Verifica o método contains
    cout << "\n Teste 3: contains " << endl;
    vector3d insidePt(1.0f, 1.0f, 1.0f);  // Ponto dentro da esfera
    vector3d outsidePt(3.0f, 0.0f, 0.0f); // Ponto fora da esfera
    cout << "Ponto " << insidePt << " está dentro da esfera? " 
              << (sphere.contains(insidePt) ? "Sim" : "Não") << endl;
    cout << "Ponto " << outsidePt << " está dentro da esfera? " 
              << (sphere.contains(outsidePt) ? "Sim" : "Não") << endl;

    // Teste 4: Verifica o método rayCast
    cout << "\n Teste 4: rayCast " << endl;
    RayInfo rayInfo(vector3d(0.0f, 0.0f, 10.0f), vector3d(0.0f, 0.0f, -1.0f));  // Raio de cima para baixo
    if (sphere.rayCast(&rayInfo)) {
        cout << "Interseção detectada!" << endl;
        cout << "Ponto de entrada: " << rayInfo.enterPoint << " (distância: " << rayInfo.enterDist << ")" << endl;
        cout << "Ponto de saída: " << rayInfo.outPoint << " (distância: " << rayInfo.outDist << ")" << endl;
    } else {
        cout << "Nenhuma interseção detectada." << endl;
    }

    // Teste 5: Verifica o método getVolume e getSurfaceArea
    cout << "\n Teste 5: Volume e Área da Superfície " << endl;
    cout << "Volume da esfera: " << sphere.getVolume() << endl;
    cout << "Área da superfície da esfera: " << sphere.getSurfaceArea() << endl;

    // Teste 6: Verifica transformações (translação, rotação e escala)
    cout << "\n Teste 6: Transformações " << endl;
    sphere.translate(vector3d(1.0f, 2.0f, 3.0f));  // Move a esfera para (1, 2, 3)
    sphere.rotate(quaternion::fromAxis(vector3d(0.0f, 1.0f, 0.0f), GPP_PI / 2));  // Rotaciona 90 graus em Y
    sphere.scale(vector3d(2.0f, 1.0f, 1.0f));  // Escala a esfera

    cout << "Após transformações:" << endl;
    cout << sphere.toString() << endl;

    // Teste 7: Verifica o método getAABB após transformações
    cout << "\n Teste 7: AABB após transformações " << endl;
    AABB aabb = sphere.getAABB();
    cout << "AABB: " << aabb.toString() << endl;
}

void testBox() {
    // Cria uma caixa com extents (1, 1, 1) e centro na origem
    Box3d box(vector3d(1.0f, 1.0f, 1.0f));

    // Teste 1: Verifica o método toString
    cout << " Teste 1: toString " << endl;
    cout << box.toString() << endl;

    // Teste 2: Verifica o método getClosestPoint
    cout << "\n Teste 2: getClosestPoint " << endl;
    vector3d pt(2.0f, 0.5f, 0.5f);  // Ponto fora da caixa
    vector3d closest = box.getClosestPoint(pt);
    cout << "Ponto mais próximo de " << pt << " é " << closest << endl;

    // Teste 3: Verifica o método contains
    cout << "\n Teste 3: contains " << endl;
    vector3d insidePt(0.5f, 0.5f, 0.5f);  // Ponto dentro da caixa
    vector3d outsidePt(2.0f, 0.0f, 0.0f);  // Ponto fora da caixa
    cout << "Ponto " << insidePt << " está dentro da caixa? "
              << (box.contains(insidePt) ? "Sim" : "Não") << endl;
    cout << "Ponto " << outsidePt << " está dentro da caixa? "
              << (box.contains(outsidePt) ? "Sim" : "Não") << endl;

    // Teste 4: Verifica o método rayCast
    cout << "\n Teste 4: rayCast " << endl;
    RayInfo rayInfo(vector3d(0.0f, 0.0f, 10.0f), vector3d(0.0f, 0.0f, -1.0f));  // Raio de cima para baixo
    if (box.rayCast(&rayInfo)) {
        cout << "Interseção detectada!" << endl;
        cout << "Ponto de entrada: " << rayInfo.enterPoint << " (distância: " << rayInfo.enterDist << ")" << endl;
        cout << "Ponto de saída: " << rayInfo.outPoint << " (distância: " << rayInfo.outDist << ")" << endl;
    } else {
        cout << "Nenhuma interseção detectada." << endl;
    }

    // Teste 5: Verifica o método getVolume e getSurfaceArea
    cout << "\n Teste 5: Volume e Área da Superfície " << endl;
    cout << "Volume da caixa: " << box.getVolume() << endl;
    cout << "Área da superfície da caixa: " << box.getSurfaceArea() << endl;

    // Teste 6: Verifica transformações (translação, rotação e escala)
    cout << "\n Teste 6: Transformações " << endl;
    box.translate(vector3d(1.0f, 2.0f, 3.0f));  // Move a caixa para (1, 2, 3)
    box.rotate(quaternion::fromAxis(vector3d(0.0f, 1.0f, 0.0f), GPP_PI / 2));  // Rotaciona 90 graus em Y
    box.scale(vector3d(2.0f, 1.0f, 1.0f));  // Escala a caixa
    cout << "Após transformações:" << endl;
    cout << box.toString() << endl;

    // Teste 7: Verifica o método getAABB após transformações
    cout << "\n Teste 7: AABB após transformações " << endl;
    AABB aabb = box.getAABB();
    cout << "AABB: " << aabb.toString() << endl;
}

void testSphereSphere() {
    cout << " Teste: Colisão entre Esferas " << endl;

    // Cria duas esferas para teste
    Sphere3d sphereA(10.0f, Transform(vector3d(0.0f, 0.0f, 0.0f)));
    Sphere3d sphereB(2.5f);
//sphereB.translate({0,0,5});

    // Verifica colisão entre as esferas
    CollisionInfo info;
    if (checkSphereSphereCollision(&sphereA, &sphereB, &info)) {
        cout << "Colisão detectada!" << endl;
        cout << info.toString() << endl;

        // Resolver a colisão (mover as esferas para fora uma da outra)
        vector3d correction = info.normal * info.depth * 0.5f;
        sphereA.translate(-correction);
        sphereB.translate(correction);
    } else {
        cout << "Nenhuma colisão detectada." << endl;
    }
}

void testCapsuleCapsule() {
    cout << " Teste: Colisão entre Cápsulas " << endl;

    // Cria duas cápsulas para teste
    Capsule3d capsuleA(vector3d(0.0f, 0.0f, 0.0f), vector3d(0.0f, 2.0f, 0.0f), 1.0f);
    Capsule3d capsuleB(vector3d(0.0, 0.0f, 0.0f), vector3d(0.00f, 2.0f, 0.0f), 1.0f);
capsuleB.translate({1.75, 0, 0});
    // Verifica colisão entre as cápsulas
    CollisionInfo info;
    if (checkCapsuleCapsuleCollision(&capsuleA, &capsuleB, &info)) {
        cout << "Colisão detectada!" << endl;
        cout << info.toString() << endl;

        // Resolver a colisão (mover as cápsulas para fora uma da outra)
        vector3d correction = info.normal * info.depth * 0.5f;
        capsuleA.translate(-correction);
        capsuleB.translate(correction);
    } else {
        cout << "Nenhuma colisão detectada." << endl;
    }
}

void testBoxBox() {
    cout << " Teste: Colisão entre Caixas " << endl;

    // Cria duas caixas para teste
    Box3d boxA(vector3d(1.0f, 1.0f, 1.0f));
    Box3d boxB(vector3d(1.0f, 1.0f, 1.0f));
boxB.translate({1.95, 0, 0});

    // Verifica colisão entre as caixas
    CollisionInfo info;
    if (checkBoxBoxCollision(&boxA, &boxB, &info)) {
        cout << "Colisão detectada!" << endl;
        cout << info.toString() << endl;

        // Resolver a colisão (mover as caixas para fora uma da outra)
        vector3d correction = info.normal * info.depth * 0.5f;
        boxA.translate(-correction);
        boxB.translate(correction);
    } else {
        cout << "Nenhuma colisão detectada." << endl;
    }


}

void testSphereCapsule() {
    cout << " Teste: Colisão entre Esfera e Cápsula " << endl;

    // Cria uma esfera e uma cápsula para teste
    Sphere3d sphere(1.0f);
    Capsule3d capsule(vector3d(1.5f, 0.0f, 0.0f), vector3d(1.5f, 2.0f, 0.0f), 1.0f);

    // Verifica colisão entre a esfera e a cápsula
    CollisionInfo info;
    if (checkSphereCapsuleCollision(&sphere, &capsule, &info)) {
        cout << "Colisão detectada!" << endl;
        cout << info.toString() << endl;

        // Resolver a colisão (mover a esfera e a cápsula para fora uma da outra)
        vector3d correction = info.normal * info.depth * 0.5f;
        sphere.translate(-correction);
        capsule.translate(correction);
    } else {
        cout << "Nenhuma colisão detectada." << endl;
    }

    cout << "==" << endl;
}
void testSphereBox() {
    cout << " Teste: Colisão entre Esfera e Caixa " << endl;

    // Cria uma esfera e uma caixa para teste
    Sphere3d sphere(1.0f);
    Box3d box(vector3d(1.0f, 1.0f, 1.0f));
box.translate({1.5f, 0, 0});
    // Verifica colisão entre a esfera e a caixa
    CollisionInfo info;
    if (checkBoxSphereCollision(&box, &sphere, &info)) {
        cout << "Colisão detectada!" << endl;
        cout << info.toString() << endl;

        // Resolver a colisão (mover a esfera e a caixa para fora uma da outra)
        vector3d correction = info.normal * info.depth * 0.5f;
        sphere.translate(-correction);
        box.translate(correction);
    } else {
        cout << "Nenhuma colisão detectada." << endl;
    }

}

void testCapsuleBox() {
    cout << " Teste: Colisão entre Cápsula e Caixa " << endl;

    // Cria uma cápsula e uma caixa para teste
    Capsule3d capsule(vector3d(1.5f, 0.0f, 0.0f), vector3d(1.5f, 2.0f, 0.0f), 1.0f);
    Box3d box(vector3d(1.0f, 1.0f, 1.0f));

    // Verifica colisão entre a cápsula e a caixa
    CollisionInfo info;
    if (checkBoxCapsuleCollision(&box, &capsule, &info)) {
        cout << "Colisão detectada!" << endl;
        cout << info.toString() << endl;

        // Resolver a colisão (mover a cápsula e a caixa para fora uma da outra)
        vector3d correction = info.normal * info.depth * 0.5f;
        capsule.translate(-correction);
        box.translate(correction);
    } else {
        cout << "Nenhuma colisão detectada." << endl;
    }

    cout << "=" << endl;
}

}
#endif
