#include <cmath>
#include <cassert>
#include <tuple>
#include <vector>

struct AVL
{
    using ll = long long;
    struct Node
    {
        ll val = 0;
        Node *l = nullptr, *r = nullptr, *p = nullptr;
        int n = 1;
        int size = 1;
        int h=1;
        void updateH()
        {
            h=1+std::max(l?l->h:0,r?r->h:0);
        }
        void updateSize()
        {
            size=n+(l?l->size:0)+(r?r->size:0);
        }
        void update()
        {
            updateH();updateSize();
        }
        int bf()
        {
            return (l?l->h:0)-(r?r->h:0);
        }
        Node* findMin()
        {
            if (!l) return this;
            else return(l->findMin());
        }
        Node*& ref()
        {
            static Node* tmp = nullptr;
            if (!p) return tmp;
            else if (p->l==this) return p->l;
            else return p->r;
        }
        void makeBalance()
        {
            int r_bf=bf();
            if (abs(r_bf)<=1) return;
            if (r_bf>1)
            {
                if (l->bf()<0)
                {
                    Node* cur=l->r;
                    rightRotate(cur);
                    leftRotate(cur);
                }
                else
                {
                    leftRotate(l);
                }
            }
            else if(r_bf<-1)
            {
                if (r->bf()>0)
                {
                    Node* cur=r->l;
                    leftRotate(cur);
                    rightRotate(cur);
                }
                else
                {
                    rightRotate(r);
                }

            }

        }
        ~Node()
        {
            delete l;
            delete r;
        }
    };

    Node *root = nullptr;
    void insert(ll new_val)
    {
        insert(root,new_val);
        while(root->p)root=root->p;
    }

    Node* insert(Node *&cur, ll new_val)
    {
        if (cur == nullptr)
        {
            cur = new Node{new_val};
            return cur;
        }
        else if (new_val < cur->val)
        {
            insert(cur->l, new_val)->p = cur;
        }
        else if (new_val == cur->val)
        {
            cur->n++;
        }
        else
        {
            insert(cur->r, new_val)->p = cur;
        }
        cur->makeBalance();
        cur->update();
        return cur;
    }
    static void rightRotate(Node* cur)
    {
        assert(cur&&cur->p&&cur->p->r==cur);
        Node* n2=cur,*n1=cur->p,*n3=cur->l,*n0=n1->p;
        if (n3)
        {
            n3->p=n1;
        }
        n1->p=n2;n1->r=n3;
        n1->update();
        n2->p=n0;n2->l=n1;
        n2->update();
        if (n0)
        {
            if (n0->l==n1)n0->l=n2;
            else n0->r=n2;
            n0->update();
        }
    };
    static void removeLLeaf(Node* cur)
    {
        assert(!cur->l);
        if(cur->r)cur->r->p=cur->p;
        if(cur->p)cur->ref()=cur->r;
        upwardBalance(cur->p);

        cur->r=nullptr;
        delete cur;
    }
    static void upwardBalance(Node* cur)
    {
        if (!cur) return;
        cur->makeBalance();
        cur->update();
        upwardBalance(cur->p);
    }
    bool del(ll val)
    {
        if (del(root,val))
        {
            while(root->p)root=root->p;
            return true;
        }
        else
        {
            return false;
        }
    }
    bool del(Node*& cur,ll val)
    {
        if (cur==nullptr) return false;
        if (cur->val==val)
        {
            if (cur->n>1)
            {
                cur->n--;
                upwardBalance(cur);
            }
            else
            {
                Node*p =cur->p;
                if(!cur->l&&!cur->r)
                {
                    delete cur;
                    cur=nullptr;
                    upwardBalance(p);
                }
                else if (cur->l==nullptr)
                {
                    auto pre=cur;
                    cur->r->p=cur->p;
                    cur=cur->r;
                    pre->r=nullptr;
                    delete pre;
                    upwardBalance(p);
                }
                else if (cur->r==nullptr)
                {
                    auto pre=cur;
                    cur->l->p=cur->p;
                    cur=cur->l;
                    pre->l=nullptr;
                    delete pre;
                    upwardBalance(p);
                }
                else
                {
                    auto ubp=cur->r->findMin();
                    std::tie(cur->val,cur->n)=std::make_tuple(ubp->val,ubp->n);
                    removeLLeaf(ubp);
                }
            }
            return true;
        }
        else if(val<cur->val)
        {
            return del(cur->l,val);
        }
        else
        {
            return del(cur->r,val);
        }
    }

    static void leftRotate(Node* cur)
    {

        assert(cur&&cur->p&&cur->p->l==cur);
        Node* n2=cur,*n1=cur->p,*n3=cur->r,*n0=n1->p;
        if (n3)
        {
            n3->p=n1;
        }
        n1->p=n2;n1->l=n3;
        n1->update();
        n2->p=n0;n2->r=n1;
        n2->update();
        if (n0)
        {
            if (n0->l==n1)n0->l=n2;
            else n0->r=n2;
            n0->update();
        }
    }
    void sort_arr(std::vector<ll>& vc)
    {
        sort_arr(vc,root);
        
    };
    void sort_arr(std::vector<ll>& vc,Node* cur)
    {
        if (cur==nullptr) return;
        sort_arr(vc,cur->l);
        for (int i=0;i<cur->n;i++) vc.emplace_back(cur->val);
        sort_arr(vc,cur->r);
    };
    ~AVL() { delete root; }
};