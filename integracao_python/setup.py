from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "sistema_recomendacao",
        ["../Similaridade.cpp", "../Recomendacao.cpp", "../CSR.cpp"],
        include_dirs=[".."], # pra pegar arquivos de fora da pasta
        cxx_std=17,
    )
]

setup(
    name="sistema_recomendacao",
    version="0.1.0",
    description="Sistema de Recomendação",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
