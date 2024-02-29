#pragma once
#include <vector>

namespace DS {

    struct Node
    {
        //��������ָ��
        Node* right;
        Node* down;
        int val;
        Node(Node* right, Node* down, int val)
            : right(right), down(down), val(val) {}
    };

    class Skiplist {
    private:
        Node* head;
        // Ԥ�ȷ������������
        const static int MAX_LEVEL = 32;
        // ����ʱ��ʱʹ�õı���
        std::vector<Node*> pathList;
    public:
        Skiplist() {
            //��ʼ��ͷ���
            head = new Node(NULL, NULL, -1);
            pathList.reserve(MAX_LEVEL);
        }

        bool search(int target)
        {
            Node* p = head;
            while (p)
            {
                // ����Ѱ��Ŀ������
                while (p->right && p->right->val < target)
                {
                    p = p->right;
                }
                // û�ҵ�Ŀ��ֵ�������������
                if (!p->right || target < p->right->val)
                {
                    p = p->down;
                }
                else
                {
                    //�ҵ�Ŀ��ֵ������
                    return true;
                }
            }
            return false;
        }

        void add(int num) {
            //�������¼�¼����·��
            pathList.clear();
            Node* p = head;
            // ���ϵ���ȥ���� ��С��num�� ���֣����ھ�������һ��ʵ����� prevs
            while (p)
            {
                // �����ҵ���С��num��p
                while (p->right && p->right->val < num)
                {
                    p = p->right;
                }
                pathList.push_back(p);
                p = p->down;
            }

            bool  insertUp = true;
            Node* downNode = NULL;
            // ������������·�����ݣ�50%����
            // ����ʵ���ǻᱣ֤���ᳬ����ǰ�Ĳ����ģ�Ȼ��ͷ���ȥ����Ӳ㣬 ��ÿ������һ��
            while (insertUp && pathList.size() > 0)
            {
                Node* insert = pathList.back();
                pathList.pop_back();
                // add�½��
                insert->right = new Node(insert->right, downNode, num);
                // ���½�㸳ֵΪdownNode
                downNode = insert->right;
                // 50%����   
                insertUp = (rand() & 1) == 0;

            }
            // �����µ�ͷ��㣬�Ӳ�
            if (insertUp)
            {

                head = new Node(new Node(NULL, downNode, num), head, -1);
            }
        }

        bool erase(int num) {
            Node* p = head;
            bool seen = false;
            while (p)
            {
                // ��ǰ�������ҵ���С�ĵ�
                while (p->right && p->right->val < num)
                {
                    p = p->right;
                }
                // ��Ч�㣬���� ̫�� ����һ��ȥ
                if (!p->right || p->right->val > num)
                {
                    p = p->down;
                }
                else
                {
                    // ������Ŀ���㣬����ɾ�������������¼Ϊtrue
                    // �������²�������ɾ��һ��Ŀ����
                    seen = true;
                    p->right = p->right->right;
                    p = p->down;
                }
            }
            return seen;
        }
    };

}