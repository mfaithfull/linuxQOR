// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_COMMON_SCENETEMPLATE
#define QOR_PP_H_FRAMEWORK_UI_COMMON_SCENETEMPLATE

//holder for scene outline as loaded from a resource or built by code
//The scene tree in terms of layout, widgets by name and CCS like style classes that might actually be CSS
namespace qor { namespace ui { 

    class SceneTemplate
    {
    public:

        SceneTemplate() = default;
        ~SceneTemplate() = default;

        /*
        FromJSON to derive a SceneTemplate from a JSON Object
        byte AddTopLevelContainer(const std::string& name)
        ref_of<TLCTemplate>::type GetTopLevelContainer(byte index);
        ref_of<SceneTemplateItem>::type GetItem(const std::string& index);
        ref_of<Scene>::type Build(Renderer* renderer);
        */

    };

    //LayerTemplate
    //SceneTemplateItem
    //TLCTemplate
    //LayoutTemplate - Name
    //WidgetTemplate - Name, Style Name

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_COMMON_SCENETEMPLATE
