from agnes import Agnes

def build_models(n):
    return [
        Agnes(n, 'single'),
        Agnes(n, 'complete'),
        Agnes(n, 'average'),
        Agnes(n, 'wards')
    ]

def pytest_generate_tests(metafunc):
    if 'single' in metafunc.fixturenames:
        metafunc.parametrize("single", build_models(1))
    if 'dual' in metafunc.fixturenames:
        metafunc.parametrize("dual", build_models(2))
    if 'triple' in metafunc.fixturenames:
        metafunc.parametrize("triple", build_models(3))
    