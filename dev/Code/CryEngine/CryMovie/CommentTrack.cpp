/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#include "StdAfx.h"
#include "CommentTrack.h"

//-----------------------------------------------------------------------------
CCommentTrack::CCommentTrack()
{
}

//-----------------------------------------------------------------------------
void CCommentTrack::GetKeyInfo(int key, const char*& description, float& duration)
{
    static char desc[128];
    assert(key >= 0 && key < (int)m_keys.size());
    CheckValid();
    description = 0;
    duration = m_keys[key].m_duration;

    cry_strcpy(desc, m_keys[key].m_strComment.c_str());

    description = desc;
}

//-----------------------------------------------------------------------------
void CCommentTrack::SerializeKey(ICommentKey& key, XmlNodeRef& keyNode, bool bLoading)
{
    if (bLoading)
    {
        // Load only in the editor for loading performance.
        if (gEnv->IsEditor())
        {
            XmlString xmlComment;
            keyNode->getAttr("comment", xmlComment);
            key.m_strComment = xmlComment;
            keyNode->getAttr("duration", key.m_duration);
            const char* strFont = keyNode->getAttr("font");
            cry_strcpy(key.m_strFont, strFont);
            keyNode->getAttr("color", key.m_color);
            keyNode->getAttr("size", key.m_size);
            int alignment = 0;
            keyNode->getAttr("align", alignment);
            key.m_align = (ICommentKey::ETextAlign)(alignment);
        }
    }
    else
    {
        XmlString xmlComment(key.m_strComment.c_str());
        keyNode->setAttr("comment", xmlComment);
        keyNode->setAttr("duration", key.m_duration);
        keyNode->setAttr("font", key.m_strFont);
        keyNode->setAttr("color", key.m_color);
        keyNode->setAttr("size", key.m_size);
        keyNode->setAttr("align", (int)key.m_align);
    }
}

void CCommentTrack::GetMemoryUsage(ICrySizer* pSizer) const
{
    pSizer->AddObject(this, sizeof(*this));
}
