# OpenGL-Render-Engine
Just for study Render Engine

So I'm happy to be contacted if anybody want to study together

项目采用了ECS架构设计，便于后续组件的扩展开发

关于RTTI：
本项目中所用到的RTTI机制并没有集成目前主流的动态库，而是自己造轮子写了一个custom_rtti的反射机制
在类和属性构造时，将其meta信息存到static成员中，后续可以通过其id查询到对应的信息

关于完成度：
- Component：
  目前有最基础的Transform 和 MeshRender，后续会增加SpriteRender等2D相关的渲染组件
- Entity：
  目前Entity没有进行拓展，所有的实例都是通过Entity手动AddComponent完成的，后续会扩展一些预设好的Entity
- System：
  实现了资源管理系统、实例管理系统、文件监听系统、和渲染系统
- Graphic：
  封装了Vertex、Texture、CubeMap、Shader、ShaderSetting、GlobalShader、Material
- Render：
  实现了 ForwardRenderPath，封装了Shadow Mapping、PostProcessing、FrameBuffer等相关功能
