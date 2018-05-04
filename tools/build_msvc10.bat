@echo off
bjam toolset=msvc-10.0 variant=debug,release threading=multi link=static runtime-link=static define=_BIND_TO_CURRENT_VCLIBS_VERSION